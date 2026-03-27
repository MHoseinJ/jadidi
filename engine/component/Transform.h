#pragma once
#include "utils/Config.h"
#include "utils/math/vector.h"

struct Transform final : Component {
    Vector2 position;
    Vector2 scale;

    Transform() : position(), scale(1.0f, 1.0f) {}
    Transform(float px, float py, float sx, float sy)
        : position(px, py), scale(sx, sy) {}

    void DeSerialize(const json &j) override {
        position.x = j["position"]["x"].get<float>();
        position.y = j["position"]["y"].get<float>();
        scale.x = j["scale"]["x"].get<float>();
        scale.y = j["scale"]["y"].get<float>();
    };
};