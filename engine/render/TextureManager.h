#pragma once
#include <SDL_render.h>
#include <SDL_ttf.h>
#include <string>
#include <SDL_image.h>
#include <unordered_map>
#include "core/Engine.h"
#include "core/Log.h"
#include <sstream>
#include <iomanip>

int initTTF();

SDL_Texture* createTextureWithText(const std::string& text, SDL_Renderer* renderer, SDL_Color color,const std::string& name, int size = 16);
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

class TextManager {
public:
    static TextManager& instance();

    static std::string generateKey(const std::string& text, const SDL_Color& color, const std::string& name, const int size) {
        std::stringstream ss;
        ss << name << "|" << size << "|"
           << static_cast<int>(color.r) << "-" << static_cast<int>(color.g) << "-" << static_cast<int>(color.b) << "|"
           << text;
        return ss.str();
    }

    SDL_Texture* get(const std::string& text, const SDL_Color color, const std::string& name, int size = 16) {
        if (text.empty()) return nullptr;

        std::string key = generateKey(text, color, name, size);

        auto it = textures.find(key);
        if (it != textures.end()) {
            it->second.refCount++;
            return it->second.texture;
        }

        SDL_Texture* texture = createTextureWithText(text, renderer, color, name, size);
        if (!texture) {
            gameLog(("CreateText failed: " + std::string(SDL_GetError())).c_str(), ERROR);
            return nullptr;
        }

        textures[key] = { texture, 1 };
        return texture;
    }

    void release(const std::string& text, const SDL_Color color, const std::string& name, int size = 16) {
        std::string key = generateKey(text, color, name, size);

        auto it = textures.find(key);
        if (it == textures.end()) return;

        it->second.refCount--;
        if (it->second.refCount <= 0) {
            SDL_DestroyTexture(it->second.texture);
            textures.erase(it);
        }
    }

private:
    struct Entry {
        SDL_Texture* texture;
        int refCount;
    };
    std::pmr::unordered_map<std::string, Entry> textures;
};