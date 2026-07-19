#include "SDLRenderer.h"
#include <algorithm>

#include "scene/GameObject.h"

#include "component/Sprite.h"
#include "component/Text.h"
#include "core/Log.h"

SDLRenderer::SDLRenderer(SDL_Renderer* renderer) 
    : sdlRenderer(renderer), meterSizeInPixels(100), dirtyList(true) {}

void SDLRenderer::init() {
    int screen_w, screen_h;
    
    SDL_GetRendererOutputSize(sdlRenderer, &screen_w, &screen_h);
    screenSize = Vector2(static_cast<float>(screen_w), static_cast<float>(screen_h));
}

void SDLRenderer::sortObjectsByZIndex(std::vector<GameObject>& objects) {
    std::sort(objects.begin(), objects.end(), [](const GameObject& a, const GameObject& b) {
        const auto aSprite = a.getComponent<Sprite>();
        const auto bSprite = b.getComponent<Sprite>();

        if (!aSprite && !bSprite) return false;
        if (!aSprite) return true;
        if (!bSprite) return false;

        const int az = aSprite->z_index;
        const int bz = bSprite->z_index;
        return az < bz;
    });
}

void SDLRenderer::beginFrame() {
    SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
    SDL_RenderClear(sdlRenderer);
}

void SDLRenderer::drawScene(std::vector<GameObject>& objects, const Camera& camera) {
    if (dirtyList && !objects.empty()) {
        sortObjectsByZIndex(objects);
    }

    for (const auto& obj : objects) {
        const auto sprite = obj.getComponent<Sprite>();
        if (!sprite || !sprite->texture || sprite->srcRect.w <= 0 || sprite->srcRect.h <= 0)
            continue;

        const int w = static_cast<int>(sprite->srcRect.w * obj.transform.scale.x * camera.zoom);
        const int h = static_cast<int>(sprite->srcRect.h * obj.transform.scale.y * camera.zoom);

        SDL_Rect dst;
        float relX = (obj.transform.position.x - camera.transform.position.x) * camera.zoom * meterSizeInPixels;
        float relY = (camera.transform.position.y - obj.transform.position.y) * camera.zoom * meterSizeInPixels;

        dst.x = static_cast<int>(relX + (screenSize.x / 2.0f) - (w / 2.0f));
        dst.y = static_cast<int>(relY + (screenSize.y / 2.0f) - (h / 2.0f));
        dst.w = w;
        dst.h = h;

        SDL_RenderCopy(sdlRenderer, sprite->texture, &sprite->srcRect, &dst);
    }

    for (const auto& obj : objects) {
        const auto text = obj.getComponent<Text>();
        if (!text || !text->texture || text->srcRect.w <= 0 || text->srcRect.h <= 0)
            continue;

        const int w = static_cast<int>(text->srcRect.w * camera.zoom);
        const int h = static_cast<int>(text->srcRect.h * camera.zoom);

        SDL_Rect dst;
        float relX = (obj.transform.position.x - camera.transform.position.x) * camera.zoom;
        float relY = (camera.transform.position.y - obj.transform.position.y) * camera.zoom;

        dst.x = static_cast<int>(relX + (screenSize.x / 2.0f) - (w / 2.0f)) * meterSizeInPixels;
        dst.y = static_cast<int>(relY + (screenSize.y / 2.0f) - (h / 2.0f)) * meterSizeInPixels;
        dst.w = w;
        dst.h = h;

        SDL_RenderCopy(sdlRenderer, text->texture, &text->srcRect, &dst);
    }
}

void SDLRenderer::endFrame() {
    renderLog(); 
    SDL_RenderPresent(sdlRenderer);
    dirtyList = false;
}