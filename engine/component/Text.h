#pragma once
#include "component/Component.h"
#include "render/FontManager.h"
#include "utils/math/vector.h"

struct Text final : Component {

    std::string text;
    int fontSize;
    std::string fontName;
    SDL_Texture *texture;
    SDL_Color color;
    SDL_Rect srcRect;
    Vector2 srcSize = {};

    [[nodiscard]] Vector2 &size() {
        srcSize.x = static_cast<float>(srcRect.w);
        srcSize.y = static_cast<float>(srcRect.h);
        return srcSize;
    }
    void OnCreate() override;
    void OnDestroy() override;
    void DeSerialize(const json &j) override;
    void Reload();

};
