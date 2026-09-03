#include "TextureManager.h"
#include "FontManager.h"
#include "core/Engine.h"
#include "core/Log.h"
#include <SDL_image.h>
#include <SDL_render.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>

using namespace std;

int initTTF() {
    return TTF_Init();
}

static ITextureBackend* g_textureBackend = nullptr;

void TextureManager::setBackend(ITextureBackend* backend) {
    this->backend = backend;
    g_textureBackend = backend;
}

TextureHandle createTextureWithText(const std::string& text, SDL_Renderer* renderer, const SDL_Color color,
                                    const std::string& name, const int size) {
    if (!renderer || !g_textureBackend) {
        gameLog("no renderer or backend found", ERROR);
        return {};
    }

    TTF_Font* font = FontManager::instance().getFont(name, size);
    if (!font)
        return {};

    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    if (!surface) {
        gameLog("Failed to create surface: " + std::string(TTF_GetError()), ERROR);
        return {};
    }

    TextureHandle handle = g_textureBackend->createFromSurface(surface);
    SDL_FreeSurface(surface);

    if (!handle.isValid()) {
        gameLog("Failed to create texture from surface", ERROR);
    }

    return handle;
}

TextureHandle createImageTexture(const std::string& path) {
    if (!renderer || !g_textureBackend) {
        gameLog("createImageTexture called before renderer/backend was initialized", ERROR);
        return {};
    }

    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        gameLog("Failed to create surface: " + std::string(IMG_GetError()), ERROR);
        return {};
    }

    TextureHandle handle = g_textureBackend->createFromSurface(surface);
    SDL_FreeSurface(surface);

    if (!handle.isValid()) {
        gameLog("Failed to create texture from surface", ERROR);
    }

    return handle;
}

TextureManager& TextureManager::instance() {
    static TextureManager inst;
    return inst;
}

TextureHandle TextureManager::get(const std::string& path) {
    if (path.empty())
        return {};
    if (!renderer || !backend) {
        gameLog("TextureManager::get called before renderer/backend was initialized", ERROR);
        return {};
    }

    auto it = textures.find(path);
    if (it != textures.end()) {
        it->second.refCount++;
        return it->second.texture;
    }

    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        gameLog("IMG_Load failed: " + std::string(IMG_GetError()), ERROR);
        return {};
    }

    // use backend to create texture
    TextureHandle handle = backend->createFromSurface(surface);
    SDL_FreeSurface(surface);

    if (!handle.isValid()) {
        gameLog("CreateTexture failed", ERROR);
        return {};
    }

    textures[path] = {handle, 1};
    return handle;
}

void TextureManager::release(const std::string& path) {
    const auto it = textures.find(path);
    if (it == textures.end())
        return;

    it->second.refCount--;
    if (it->second.refCount <= 0) {
        // Use backend to destroy texture
        if (backend) {
            backend->destroyTexture(it->second.texture);
        }
        textures.erase(it);
    }
}

TextManager& TextManager::instance() {
    static TextManager inst;
    return inst;
}

TextureHandle TextManager::get(const std::string& text, const SDL_Color color, const std::string& name, int size) {
    if (text.empty())
        return {};
    std::string key = generateKey(text, color, name, size);
    auto it = textures.find(key);
    if (it != textures.end()) {
        it->second.refCount++;
        return it->second.texture;
    }

    TextureHandle handle = createTextureWithText(text, renderer, color, name, size);
    if (!handle.isValid()) {
        gameLog("CreateText failed", ERROR);
        return {};
    }
    textures[key] = {handle, 1};
    return handle;
}

void TextManager::release(const std::string& text, const SDL_Color color, const std::string& name, int size) {
    std::string key = generateKey(text, color, name, size);
    auto it = textures.find(key);
    if (it == textures.end())
        return;

    it->second.refCount--;
    if (it->second.refCount <= 0) {
        ITextureBackend* backend = TextureManager::instance().getBackend();
        if (backend) {
            backend->destroyTexture(it->second.texture);
        }
        textures.erase(it);
    }
}