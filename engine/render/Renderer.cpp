#include <SDL_render.h>
#include "Renderer.h"
#include <vector>

#include "core/Log.h"

void drawObjects(SDL_Renderer* renderer, const std::vector<SDL_Texture*>& textures) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (SDL_Texture* texture : textures) {
        if (texture != nullptr) {
            SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        }
    }

    renderLog();

    SDL_RenderPresent(renderer);
}

void drawObjects(SDL_Renderer* renderer, std::nullptr_t) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    renderLog();

    SDL_RenderPresent(renderer);
}