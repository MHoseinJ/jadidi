#include "TextureManager.h"
#include <SDL_render.h>
#include <SDL_ttf.h>
#include <iostream>
#include <SDL_image.h>
#include <string>
#include "FontManager.h"
#include "core/Engine.h"
#include "core/Log.h"

using namespace std;

int initTTF() {
    return TTF_Init();
}


static TextureHandle wrapTexture(SDL_Texture* tex) {
    TextureHandle handle;
    handle.sdlTexture = tex;
    if (tex) {
        SDL_QueryTexture(tex, nullptr, nullptr, &handle.width, &handle.height);
    }
    return handle;
}

TextureHandle createTextureWithText(const std::string& text, SDL_Renderer* renderer,
                                    const SDL_Color color, const std::string& name,
                                    const int size) {
    if (!renderer) {
        gameLog("no renderer found", ERROR);
        return {};
    }
    TTF_Font* font = FontManager::instance().getFont(name, size);
    if (!font) return {};

    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    if (!surface) {
        std::cerr << "Failed to create surface: " << TTF_GetError() << std::endl;
        return {};
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        return {};
    }

    return wrapTexture(texture);
}

TextureHandle createImageTexture(const std::string &path) {
    if (!renderer) {
        gameLog("createImageTexture called before renderer was initialized", ERROR);
        return {};
    }
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        gameLog("Failed to create surface: " + std::string(IMG_GetError()), ERROR);
        return {};
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        gameLog("Failed to create texture: " + std::string(SDL_GetError()), ERROR);
        return {};
    }

    return wrapTexture(texture);
}

TextureManager &TextureManager::instance() {
    static TextureManager inst;
    return inst;
}

TextureHandle TextureManager::get(const std::string &path) {
    if (path.empty()) return {};
    if (!renderer) {
        gameLog("TextureManager::get called before renderer was initialized", ERROR);
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

    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!tex) {
        gameLog("CreateTexture failed: " + std::string(SDL_GetError()), ERROR);
        return {};
    }

    TextureHandle handle = wrapTexture(tex);
    textures[path] = { handle, 1 };
    return handle;
}

void TextureManager::release(const std::string &path) {
    const auto it = textures.find(path);
    if (it == textures.end()) return;

    it->second.refCount--;
    if (it->second.refCount <= 0) {
        if (it->second.texture.sdlTexture) {
            SDL_DestroyTexture(it->second.texture.sdlTexture);
        }
        textures.erase(it);
    }
}


TextManager &TextManager::instance() {
    static TextManager inst;
    return inst;
}

TextureHandle TextManager::get(const std::string& text, const SDL_Color color, const std::string& name, int size) {
    if (text.empty()) return {};
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
    textures[key] = { handle, 1 };
    return handle;
}

void TextManager::release(const std::string& text, const SDL_Color color, const std::string& name, int size) {
    std::string key = generateKey(text, color, name, size);
    auto it = textures.find(key);
    if (it == textures.end()) return;

    it->second.refCount--;
    if (it->second.refCount <= 0) {
        if (it->second.texture.sdlTexture) {
            SDL_DestroyTexture(it->second.texture.sdlTexture);
        }
        textures.erase(it);
    }
}