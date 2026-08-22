#pragma once
#include <SDL_render.h>
#include <SDL_ttf.h>
#include <string>
#include <SDL_image.h>
#include <unordered_map>
#include <sstream>
#include "core/Engine.h"
#include "core/Log.h"
#include "TextureHandle.h"
#include "ITextureBackend.h"

int initTTF();

TextureHandle createTextureWithText(const std::string& text, SDL_Renderer* renderer, SDL_Color color, const std::string& name, int size = 16);
TextureHandle createImageTexture(const std::string& path);

class TextureManager {
public:
    static TextureManager& instance();
    
    void setBackend(ITextureBackend* backend);
    ITextureBackend* getBackend() const { return backend; }
    
    TextureHandle get(const std::string& path);
    void release(const std::string& path);

private:
    struct Entry {
        TextureHandle texture;
        int refCount;
    };
    
    std::pmr::unordered_map<std::string, Entry> textures;
    ITextureBackend* backend = nullptr;
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
    
    TextureHandle get(const std::string& text, const SDL_Color color, const std::string& name, int size = 16);
    void release(const std::string& text, const SDL_Color color, const std::string& name, int size = 16);

private:
    struct Entry {
        TextureHandle texture;
        int refCount;
    };
    std::pmr::unordered_map<std::string, Entry> textures;
};