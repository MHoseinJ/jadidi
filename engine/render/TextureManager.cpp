#include "TextureManager.h"

#include <SDL_render.h>
#include <SDL_ttf.h>
#include <bits/ostream.tcc>
#include <iostream>
#include <SDL_image.h>
#include <string>
#include <unordered_map>

#include "core/Engine.h"
#include "core/Log.h"

using namespace std;

TTF_Font* font;

int initFont() {
    font = TTF_OpenFont("font.ttf", 16);
    cout << "loading font..." << endl;
    if (!font) {
        cout << "Error loading font" << endl;
        return -1;
    }
    cout << "font loaded" << endl;
    return 0;
}

// legacy and unsafe codes down

SDL_Texture* createTextureWithText(const std::string& text, SDL_Renderer* renderer, const SDL_Color color) {
    if (!renderer || !font) return nullptr;

    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    if (!surface) {
        std::cerr << "Failed to create surface: " << TTF_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
    }

    return texture;
}

SDL_Texture* createImageTexture(const std::string &path) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        gameLog(("Failed to create surface: " + std::string(IMG_GetError())).c_str(), ERROR);
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);

    if (!texture) {
        gameLog(("Failed to create texture: " + std::string(SDL_GetError())).c_str(), ERROR);
    }

    return texture;
}

// working on them

TextureManager &TextureManager::instance() {
    static TextureManager inst;
    return inst;
}

SDL_Texture *TextureManager::get(const std::string &path) {
    if (path.empty()) return nullptr;

    auto it = textures.find(path);
    if (it != textures.end()) {
        it->second.refCount++;
        return it->second.texture;
    }

    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        gameLog(("IMG_Load failed: " + std::string(IMG_GetError())).c_str(), ERROR);
        return nullptr;
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!tex) {
        gameLog(("CreateTexture failed: " + std::string(SDL_GetError())).c_str(), ERROR);
        return nullptr;
    }

    textures[path] = { tex, 1 };
    return tex;
}

void TextureManager::release(const std::string &path) {
    const auto it = textures.find(path);
    if (it == textures.end()) return;

    it->second.refCount--;
    if (it->second.refCount <= 0) {
        SDL_DestroyTexture(it->second.texture);
        textures.erase(it);
    }
}
