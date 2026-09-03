#pragma once
#include <SDL_render.h>
#include <glad/glad.h>

struct TextureHandle {
    SDL_Texture* sdlTexture = nullptr;
    GLuint glTexture = 0;
    int width = 0;
    int height = 0;

    bool isValid() const {
        return sdlTexture != nullptr || glTexture != 0;
    }
};