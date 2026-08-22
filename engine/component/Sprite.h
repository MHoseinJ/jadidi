#pragma once
#include <iostream>
#include <string>
#include "Component.h"
#include "render/TextureManager.h"
#include "utils/math/vector.h"
#include "render/TextureHandle.h"

struct Sprite final : Component {
    std::string path;
    int z_index = 0;
    TextureHandle texture;
    SDL_Rect srcRect{0, 0, 0, 0};
    Vector2 srcSize{};

    void OnCreate() override {
        if (!path.empty()) {
            texture = TextureManager::instance().get(path);
        }
        if (texture.isValid()) {
            // dimensions stored in Handle
            if (srcRect.w == 0 || srcRect.h == 0) {
                srcRect = {0, 0, texture.width, texture.height};
            }
        }
    }

    void OnDestroy() override {
        if (texture.isValid() && !path.empty()) {
            TextureManager::instance().release(path);
        }
        texture = TextureHandle{}; // Reset
    }

    void Reload() {
        OnDestroy();
        OnCreate();
    }

    void SetPath(const std::string& newPath) {
        if (path == newPath) return;
        OnDestroy();
        path = newPath;
        OnCreate();
    }

    [[nodiscard]] Vector2& size() {
        srcSize.x = static_cast<float>(srcRect.w);
        srcSize.y = static_cast<float>(srcRect.h);
        return srcSize;
    }

    void DeSerialize(const json& j) override {
        path = j.value("texture", "");
        z_index = j.value("z_index", 0);
        if (j.contains("src")) {
            srcRect.x = j["src"].value("x", 0);
            srcRect.y = j["src"].value("y", 0);
            srcRect.w = j["src"].value("w", 0);
            srcRect.h = j["src"].value("h", 0);
        }
    }
};