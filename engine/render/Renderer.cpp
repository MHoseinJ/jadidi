#include "Renderer.h"
#include <algorithm>
#include "core/Log.h"
#include "component/Sprite.h"
#include "component/Text.h"
#include "core/Engine.h"

Vector2 screen_size;

void initRenderer() {
    int screen_w, screen_h;
    SDL_GetWindowSize(window, &screen_w, &screen_h);
    screen_size = Vector2(static_cast<float>(screen_w), static_cast<float>(screen_h));
}

void drawObjects(SDL_Renderer* renderer, std::vector<GameObject>& objects, const Camera& camera) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // sort by z_index if dirty
    if (dirtyList && !objects.empty()) {
        std::sort(objects.begin(), objects.end(), [](GameObject& a, GameObject& b) {
            const auto aSprite = a.getComponent<Sprite>();
            const auto bSprite = b.getComponent<Sprite>();

            if (!aSprite && !bSprite) return false;
            if (!aSprite) return true;
            if (!bSprite) return false;

            const int az = aSprite ? aSprite->z_index : 0;
            const int bz = bSprite ? bSprite->z_index : 0;

            return az < bz;
        });
    }

    for (auto& obj : objects) {
        const auto sprite = obj.getComponent<Sprite>();
        if (!sprite || !sprite->texture || sprite->srcRect.w <= 0 || sprite->srcRect.h <= 0)
            continue;

        const int w = static_cast<int>(
            sprite->srcRect.w * obj.transform.scale.x * camera.zoom
        );
        const int h = static_cast<int>(
            sprite->srcRect.h * obj.transform.scale.y * camera.zoom
        );

        SDL_Rect dst;

        float relX = (obj.transform.position.x - camera.transform.position.x) * camera.zoom;
        float relY = (camera.transform.position.y - obj.transform.position.y) * camera.zoom;

        dst.x = static_cast<int>(relX + (screen_size.x / 2.0f));
        dst.y = static_cast<int>(relY + (screen_size.y / 2.0f));

        dst.w = w;
        dst.h = h;

        SDL_RenderCopy(
            renderer,
            sprite->texture,
            &sprite->srcRect,
            &dst
        );
    }

    for (auto& obj : objects) {
        const auto text = obj.getComponent<Text>();
        if (!text || !text->texture || text->srcRect.w <= 0 || text->srcRect.h <= 0)
            continue;

        const int w = static_cast<int>(
            text->srcRect.w * camera.zoom
        );
        const int h = static_cast<int>(
            text->srcRect.h * camera.zoom
        );

        SDL_Rect dst;

        float relX = (obj.transform.position.x - camera.transform.position.x) * camera.zoom;
        float relY = (camera.transform.position.y - obj.transform.position.y) * camera.zoom;

        dst.x = static_cast<int>(relX + (screen_size.x / 2.0f));
        dst.y = static_cast<int>(relY + (screen_size.y / 2.0f));

        dst.w = w;
        dst.h = h;

        SDL_RenderCopy(
            renderer,
            text->texture,
            &text->srcRect,
            &dst
        );
    }


    renderLog();
    SDL_RenderPresent(renderer);
    dirtyList = false;
}

void drawObjects(SDL_Renderer* renderer, std::nullptr_t) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    renderLog();
    SDL_RenderPresent(renderer);
}