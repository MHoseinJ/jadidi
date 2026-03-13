#pragma once
#include <SDL_ttf.h>
#include <string>
#include <unordered_map>

class FontManager {
public:
    static FontManager& instance () {
        static FontManager instance;
        return instance;
    }

    TTF_Font *loadFont(const std::string& fontName, int fontSize);
    TTF_Font *getFont(const std::string& fontName, int fontSize);
    void clean();

private:
    std::unordered_map<std::string, TTF_Font*> fonts;
};
