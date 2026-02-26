#pragma once
#include <SDL_render.h>
#include "Component.h"
#include "render/TextureManager.h"

struct Sprite final : Component {
    std::string path;
    int z_index = 0;

    SDL_Texture* texture = nullptr;
    SDL_Rect srcRect {0, 0, 0, 0};

    int texW = 0;
    int texH = 0;

    void OnCreate() override {
        if (!path.empty()) {
            texture = TextureManager::instance().get(path);
        }

        if (texture) {
            SDL_QueryTexture(texture, nullptr, nullptr, &texW, &texH);

            if (srcRect.w == 0 || srcRect.h == 0) {
                srcRect = {0, 0, texW, texH};
            }
        }
    }

    void OnDestroy() override {
        if (!path.empty()) {
            TextureManager::instance().release(path);
        }
        texture = nullptr;
    }

    void Reload() {
        OnDestroy();
        OnCreate();
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