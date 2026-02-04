#include "Renderer.h"

#include <algorithm>
#include <SDL_render.h>
#include <vector>

#include "core/Log.h"
#include "scene/GameObject.h"

void drawObjects(SDL_Renderer* renderer, std::vector<GameObject>& objects, Camera& camera) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (dirtyList && !objects.empty()) {
        std::sort(objects.begin(), objects.end(), [](const GameObject& a, const GameObject& b) {
            return a.sprite.z_index < b.sprite.z_index;
        });
    }

    for (const auto& obj : objects) {
        if (obj.sprite.texture == nullptr)
            continue;

        int w, h;
        SDL_QueryTexture(obj.sprite.texture, nullptr, nullptr, &w, &h);

        w = static_cast<int>(w * obj.transform.scale.x * camera.zoom);
        h = static_cast<int>(h * obj.transform.scale.y * camera.zoom);

        SDL_Rect dst;
        dst.x = static_cast<int>((obj.transform.position.x - camera.transform.position.x) * camera.zoom);
        dst.y = static_cast<int>((obj.transform.position.y - camera.transform.position.y) * camera.zoom);

        dst.w = w;
        dst.h = h;

        SDL_RenderCopy(renderer, obj.sprite.texture, nullptr, &dst);
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