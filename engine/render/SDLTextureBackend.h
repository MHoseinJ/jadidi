#pragma once
#include "ITextureBackend.h"

class SDLTextureBackend : public ITextureBackend {
public:
    explicit SDLTextureBackend(SDL_Renderer* renderer);
    
    TextureHandle createFromSurface(SDL_Surface* surface) override;
    void destroyTexture(TextureHandle& handle) override;

private:
    SDL_Renderer* sdlRenderer;
};