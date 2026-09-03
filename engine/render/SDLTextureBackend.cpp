#include "SDLTextureBackend.h"
#include "core/Log.h"

SDLTextureBackend::SDLTextureBackend(SDL_Renderer* renderer) : sdlRenderer(renderer) {}

TextureHandle SDLTextureBackend::createFromSurface(SDL_Surface* surface) {
    TextureHandle handle;

    if (!sdlRenderer || !surface) {
        return handle;
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(sdlRenderer, surface);
    if (!tex) {
        gameLog("Failed to create SDL texture: " + std::string(SDL_GetError()), ERROR);
        return handle;
    }

    handle.sdlTexture = tex;

    // Query dimensions once and store in handle
    SDL_QueryTexture(tex, nullptr, nullptr, &handle.width, &handle.height);

    return handle;
}

void SDLTextureBackend::destroyTexture(TextureHandle& handle) {
    if (handle.sdlTexture) {
        SDL_DestroyTexture(handle.sdlTexture);
        handle.sdlTexture = nullptr;
        handle.width = 0;
        handle.height = 0;
    }
}