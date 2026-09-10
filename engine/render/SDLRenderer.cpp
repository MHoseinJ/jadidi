#include "SDLRenderer.h"
#include "component/Sprite.h"
#include "component/Text.h"
#include "core/Log.h"
#include "core/Units.h"
#include <algorithm>

SDLRenderer::SDLRenderer(SDL_Renderer* renderer) : sdlRenderer(renderer), dirtyList(true) {}

void SDLRenderer::init() {
    int screen_w, screen_h;
    SDL_GetRendererOutputSize(sdlRenderer, &screen_w, &screen_h);
    screenSize = Vector2(static_cast<float>(screen_w), static_cast<float>(screen_h));
}

void SDLRenderer::sortObjectsByZIndex() {
    std::sort(renderList.begin(), renderList.end(), [](GameObject* a, GameObject* b) {
        const auto aSprite = a->getComponent<Sprite>();
        const auto bSprite = b->getComponent<Sprite>();
        if (!aSprite && !bSprite)
            return false;
        if (!aSprite)
            return true;
        if (!bSprite)
            return false;
        return aSprite->z_index < bSprite->z_index;
    });
}

void SDLRenderer::beginFrame() {
    SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
    SDL_RenderClear(sdlRenderer);
}

void SDLRenderer::drawScene(std::vector<std::unique_ptr<GameObject>>& objects, const Camera& camera) {
    renderList.clear();
    renderList.reserve(objects.size());
    for (auto& obj : objects) {
        renderList.push_back(obj.get());
    }

    if (dirtyList && !renderList.empty()) {
        sortObjectsByZIndex();
    }

    // draw sprites
    for (const auto* obj : renderList) {
        const auto sprite = obj->getComponent<Sprite>();
        if (!sprite || sprite->srcRect.w <= 0 || sprite->srcRect.h <= 0)
            continue;

        const int w = static_cast<int>(sprite->srcRect.w * obj->transform.scale.x * camera.zoom);
        const int h = static_cast<int>(sprite->srcRect.h * obj->transform.scale.y * camera.zoom);

        SDL_Rect dst;
        float relX = (obj->transform.position.x - camera.transform.position.x) * camera.zoom * Units::PixelsPerMeter;
        float relY = (camera.transform.position.y - obj->transform.position.y) * camera.zoom * Units::PixelsPerMeter;

        dst.x = static_cast<int>(relX + (screenSize.x / 2.0f) - (w / 2.0f));
        dst.y = static_cast<int>(relY + (screenSize.y / 2.0f) - (h / 2.0f));
        dst.w = w;
        dst.h = h;

        if (sprite->hasTexture && sprite->texture.sdlTexture) {
            SDL_Point center = {w / 2, h / 2};
            SDL_RenderCopyEx(sdlRenderer, sprite->texture.sdlTexture, &sprite->srcRect, &dst,
                             obj->transform.rotation, &center, SDL_FLIP_NONE);
        } else {
            float cx = dst.x + dst.w / 2.0f;
            float cy = dst.y + dst.h / 2.0f;
            float rad = obj->transform.rotation * 3.14159265358979f / 180.0f;
            float cosR = std::cos(rad);
            float sinR = std::sin(rad);
        
            auto rotatePoint = [&](float px, float py) -> SDL_FPoint {
                float dx = px - cx;
                float dy = py - cy;
                return {
                    cx + dx * cosR - dy * sinR,
                    cy + dx * sinR + dy * cosR
                };
            };
        
            SDL_FPoint tl = rotatePoint(static_cast<float>(dst.x),           static_cast<float>(dst.y));
            SDL_FPoint tr = rotatePoint(static_cast<float>(dst.x + dst.w),   static_cast<float>(dst.y));
            SDL_FPoint bl = rotatePoint(static_cast<float>(dst.x),           static_cast<float>(dst.y + dst.h));
            SDL_FPoint br = rotatePoint(static_cast<float>(dst.x + dst.w),   static_cast<float>(dst.y + dst.h));
        
            SDL_Vertex vertices[] = {
                {tl, sprite->color, {0.0f, 0.0f}},
                {tr, sprite->color, {0.0f, 0.0f}},
                {bl, sprite->color, {0.0f, 0.0f}},
                {br, sprite->color, {0.0f, 0.0f}}
            };
        
            int indices[] = {0, 1, 2, 1, 3, 2};
        
            SDL_RenderGeometry(sdlRenderer, nullptr, vertices, 4, indices, 6);
        }
    }

    // draw texts
    for (const auto* obj : renderList) {
        const auto text = obj->getComponent<Text>();
        if (!text || !text->texture.isValid() || text->srcRect.w <= 0 || text->srcRect.h <= 0)
            continue;

        const int w = static_cast<int>(text->srcRect.w * camera.zoom);
        const int h = static_cast<int>(text->srcRect.h * camera.zoom);

        SDL_Rect dst;
        float relX = (obj->transform.position.x - camera.transform.position.x) * camera.zoom * Units::PixelsPerMeter;
        float relY = (camera.transform.position.y - obj->transform.position.y) * camera.zoom * Units::PixelsPerMeter;

        dst.x = static_cast<int>(relX + (screenSize.x / 2.0f) - (w / 2.0f));
        dst.y = static_cast<int>(relY + (screenSize.y / 2.0f) - (h / 2.0f));
        dst.w = w;
        dst.h = h;

        SDL_Point center = {w / 2, h / 2};
        SDL_RenderCopyEx(sdlRenderer, text->texture.sdlTexture, &text->srcRect, &dst,
                         obj->transform.rotation, &center, SDL_FLIP_NONE);
    }
}

void SDLRenderer::endFrame() {
    renderLog();
    SDL_RenderPresent(sdlRenderer);
    dirtyList = false;
}

void SDLRenderer::renderLogs(int g_textures_created, int height) {
    for (size_t i = 0; i < AllLogs.size(); i++) {
        auto& entry = AllLogs[i];

        if (!entry.texture.isValid()) {
            entry.texture = createTextureWithText(entry.message, renderer, chooseColor(entry.type), "font", 16);
            if (entry.texture.isValid())
                ++g_textures_created;
            if (!entry.texture.isValid())
                continue;
        }

        SDL_Rect rect;
        rect.w = entry.texture.width;
        rect.h = entry.texture.height;
        rect.y = height - (static_cast<int>(i) * (rect.h + 5) + 50);
        rect.x = 25;

        if (entry.texture.sdlTexture) {
            SDL_RenderCopy(renderer, entry.texture.sdlTexture, nullptr, &rect);
        }
    }
}

void SDLRenderer::resize(int width, int height) {
    screenSize = Vector2(static_cast<float>(width), static_cast<float>(height));
    SDL_RenderSetLogicalSize(sdlRenderer, width, height);
}