#pragma once
#include "component/Component.h"
#include "render/FontManager.h"

struct Text final : Component {

    std::string text;
    int fontSize;
    std::string fontName;
    SDL_Texture *texture;
    SDL_Color color;
    SDL_Rect srcRect;

    void OnCreate() override;
    void OnDestroy() override;
    void DeSerialize(const json &j) override;
    void Reload();

};
