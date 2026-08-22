#pragma once
#include <SDL_render.h>

struct TextureHandle {
    SDL_Texture* sdlTexture = nullptr; // will become unsigned int later
    int width = 0;
    int height = 0;

    bool isValid() const { return sdlTexture != nullptr; }
};