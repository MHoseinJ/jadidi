#include "GLTextureBackend.h"
#include "core/Log.h"
#include <glad/glad.h>

GLTextureBackend::GLTextureBackend() {}

TextureHandle GLTextureBackend::createFromSurface(SDL_Surface* surface) {
    TextureHandle handle;

    if (!surface) {
        gameLog("GLTextureBackend: surface is null", ERROR);
        return handle;
    }

    handle.width = surface->w;
    handle.height = surface->h;

    SDL_Surface* convertedSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);

    if (!convertedSurface) {
        gameLog("GLTextureBackend: failed to convert surface to RGBA: " + std::string(SDL_GetError()), ERROR);
        return handle;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    float borderColor[4] = {0, 0, 0, 0};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, convertedSurface->w, convertedSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 convertedSurface->pixels);

    SDL_FreeSurface(convertedSurface);
    glBindTexture(GL_TEXTURE_2D, 0);

    handle.glTexture = textureID;

    return handle;
}

void GLTextureBackend::destroyTexture(TextureHandle& handle) {
    if (handle.glTexture != 0) {
        glDeleteTextures(1, &handle.glTexture);
        handle.glTexture = 0;
        handle.width = 0;
        handle.height = 0;
    }
}