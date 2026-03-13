#include "FontManager.h"

#include <iostream>
#include <bits/ostream.tcc>

#include "core/Log.h"

TTF_Font *FontManager::loadFont(const std::string &fontName, int fontSize) {
    const std::string path = "Fonts/" + fontName + ".ttf";
    TTF_Font* font = TTF_OpenFont(path.c_str(), fontSize);

    if (!font) {
        gameLog("unable to load font: " + fontName + " Error: " + TTF_GetError(), ERROR);
        return nullptr;
    }
    std::string new_name = fontName + "_" + std::to_string(fontSize);
    fonts[new_name] = font;
    return font;
}

TTF_Font* FontManager::getFont(const std::string& fontName, const int fontSize) {

    std::string key = fontName + "_" + std::to_string(fontSize);


    if (const auto it = fonts.find(key); it != fonts.end()) {
        return it->second;
    }

    TTF_Font* font = loadFont(fontName, fontSize);
    return font;
}

void FontManager::clean() {
    for (auto&[fst, snd] : fonts) {
        TTF_CloseFont(snd);
    }
    fonts.clear();
}
