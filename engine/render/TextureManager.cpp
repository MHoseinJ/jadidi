#include "TextureManager.h"

#include <SDL_render.h>
#include <SDL_ttf.h>
#include <bits/ostream.tcc>
#include <iostream>
#include <SDL_image.h>
#include <string>

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

SDL_Texture* createImageTexture(const std::string &path, SDL_Renderer *renderer) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        gameLog(("Failed to create surface: " + std::string(TTF_GetError())).c_str(), ERROR);
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);

    if (!texture) {
        gameLog(("Failed to create texture: " + std::string(SDL_GetError())).c_str(), ERROR);
    }

    return texture;
}
