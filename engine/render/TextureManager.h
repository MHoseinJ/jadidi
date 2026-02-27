#pragma once
#include <SDL_render.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>
#include <SDL_image.h>
#include <unordered_map>

#include "core/Engine.h"
#include "core/Log.h"

extern TTF_Font* font;

int initFont();

SDL_Texture* createTextureWithText(const std::string& text, SDL_Renderer* renderer, SDL_Color color);
SDL_Texture* createImageTexture(const std::string& path);


class TextureManager {
public:
    static TextureManager& instance();

    SDL_Texture* get(const std::string& path);
    void release(const std::string& path);
private:
    struct Entry {
        SDL_Texture* texture;
        int refCount;
    };

    std::pmr::unordered_map<std::string, Entry> textures;
};