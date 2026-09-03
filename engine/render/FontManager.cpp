#include "FontManager.h"

#include <cstdlib>
#include <iostream>

#include <SDL2/SDL_ttf.h>

#include "core/Input.h"
#include "core/Log.h"

TTF_Font* FontManager::loadFont(const std::string& fontName, const int fontSize) {
    const std::string path = "Fonts/" + fontName + ".ttf";

    TTF_Font* font = TTF_OpenFont(path.c_str(), fontSize);

    if (!font) {
        gameLog("Unable to load font: " + path, ERROR);
        gameLog("TTF error: " + std::string(TTF_GetError()), ERROR);
        gameLog("Please place a valid .ttf font file at: " + path, ERROR);

        Input::Quit();
    }

    const std::string new_name = fontName + "_" + std::to_string(fontSize);
    fonts[new_name] = font;

    return font;
}

TTF_Font* FontManager::getFont(const std::string& fontName, const int fontSize) {
    const std::string key = fontName + "_" + std::to_string(fontSize);

    if (const auto it = fonts.find(key); it != fonts.end()) {
        return it->second;
    }

    TTF_Font* font = loadFont(fontName, fontSize);
    return font;
}

void FontManager::clean() {
    for (auto& [name, font] : fonts) {
        if (font) {
            TTF_CloseFont(font);
        }
    }

    fonts.clear();
    TTF_Quit();
}