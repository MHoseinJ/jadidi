#pragma once
#include "Component.h"
#include "render/TextureHandle.h"
#include "render/TextureManager.h"
#include "utils/math/vector.h"
#include <iostream>
#include <string>

struct Sprite final : Component {
    std::string path;
    int z_index = 0;
    TextureHandle texture;
    SDL_Rect srcRect{0, 0, 0, 0};
    Vector2 srcSize{};
    SDL_Color color{255, 255, 255, 255};
    bool hasTexture = false;

    void OnCreate() override {
        if (!path.empty()) {
            texture = TextureManager::instance().get(path);
            if (texture.isValid()) {
                hasTexture = true;
                if (srcRect.w == 0 || srcRect.h == 0) {
                    srcRect = {0, 0, texture.width, texture.height};
                }
            }
        } else {
            hasTexture = false;
            if (srcRect.w == 0 || srcRect.h == 0) {
                srcRect = {0, 0, 16, 16};
            }
        }
    }

    void OnDestroy() override {
        if (hasTexture && !path.empty()) {
            TextureManager::instance().release(path);
        }
        texture = TextureHandle{};
        hasTexture = false;
    }

    void Reload() {
        OnDestroy();
        OnCreate();
    }

    void SetPath(const std::string& newPath) {
        if (path == newPath)
            return;
        OnDestroy();
        path = newPath;
        OnCreate();
    }

    [[nodiscard]] Vector2& size() {
        srcSize.x = static_cast<float>(srcRect.w);
        srcSize.y = static_cast<float>(srcRect.h);
        return srcSize;
    }

    void DeSerialize(const Json& j) override {
        path = j.get<std::string>("texture", "");
        z_index = j.get<int>("z_index", 0);
        
        if (j.has("color")) {
            Json colorJson = j.getObject("color");
            color.r = colorJson.get<int>("r", 255);
            color.g = colorJson.get<int>("g", 255);
            color.b = colorJson.get<int>("b", 255);
            color.a = colorJson.get<int>("a", 255);
        }
        
        if (j.has("src")) {
            Json srcJson = j.getObject("src");
            srcRect.x = srcJson.get<int>("x", 0);
            srcRect.y = srcJson.get<int>("y", 0);
            srcRect.w = srcJson.get<int>("w", 0);
            srcRect.h = srcJson.get<int>("h", 0);
        }
    }
};