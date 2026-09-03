#pragma once
#include "utils/Config.h"
#include "utils/math/vector.h"
#include "Component.h"

struct Transform final : Component {
    Vector2 position;
    Vector2 scale;

    Transform() : position(), scale(1.0f, 1.0f) {}
    Transform(float px, float py, float sx, float sy) : position(px, py), scale(sx, sy) {}

    void DeSerialize(const json& j) override {
        if (j.contains("position")) {
            position.x = j["position"].value("x", 0.0f);
            position.y = j["position"].value("y", 0.0f);
        }
        if (j.contains("scale")) {
            scale.x = j["scale"].value("x", 1.0f);
            scale.y = j["scale"].value("y", 1.0f);
        }
    }
};