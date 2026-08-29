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

    GLenum format;
    if (surface->format->BytesPerPixel == 4) {
        format = GL_RGBA;
    } else if (surface->format->BytesPerPixel == 3) {
        format = GL_RGB;
    } else {
        gameLog("GLTextureBackend: unsupported surface format", ERROR);
        return handle;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        format,
        surface->w,
        surface->h,
        0,
        format,
        GL_UNSIGNED_BYTE,
        surface->pixels
    );

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