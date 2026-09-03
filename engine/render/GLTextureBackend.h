#pragma once
#include "ITextureBackend.h"

class GLTextureBackend : public ITextureBackend {
  public:
    GLTextureBackend();

    TextureHandle createFromSurface(SDL_Surface* surface) override;
    void destroyTexture(TextureHandle& handle) override;
};