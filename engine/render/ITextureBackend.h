#pragma once
#include "TextureHandle.h"
#include <SDL2/SDL.h>
#include <string>

class ITextureBackend {
  public:
    virtual ~ITextureBackend() = default;

    // Create texture from SDL_Surface
    virtual TextureHandle createFromSurface(SDL_Surface* surface) = 0;

    // Destroy texture by handle
    virtual void destroyTexture(TextureHandle& handle) = 0;
};