#include "Text.h"

#include "render/TextureManager.h"

void Text::OnCreate() {

    if (!text.empty() && fontSize && !fontName.empty()) {
        texture = createTextureWithText(text, renderer, color, fontName, fontSize);
    }

    if (texture) {
        SDL_QueryTexture(texture, nullptr, nullptr, &srcRect.w, &srcRect.h);
    }

}

void Text::OnDestroy() {
    SDL_DestroyTexture(texture);
}

void Text::DeSerialize(const json &j) {
    if (j.find("text") != j.end()) {
        text = j["text"];
    }
    if (j.find("color") != j.end()) {
        color.r = j["color"]["r"];
        color.g = j["color"]["g"];
        color.b = j["color"]["b"];
        color.a = j["color"]["a"];
    } else {
        color.r = 0;
        color.g = 0;
        color.b = 0;
        color.a = 255;
    }
    if (j.find("size") != j.end()) {
        fontSize = j["size"];
    } else {
        fontSize = 16;
    }
    if (j.find("font") != j.end()) {
        fontName = j["font"];
    } else {
        fontName = "font";
    }
}

void Text::Reload() {
    OnDestroy();
    OnCreate();
}
