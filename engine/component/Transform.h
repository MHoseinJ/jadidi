#pragma once
#include "utils/Config.h"
#include "utils/math/vector.h"
#include "Component.h"

struct Transform final : Component {
    Vector2 position;
    Vector2 scale;
    float rotation = 0.0f;

    Transform() : position(), scale(1.0f, 1.0f), rotation(0.0f) {}
    Transform(float px, float py, float sx, float sy, float rot = 0.0f)
        : position(px, py), scale(sx, sy), rotation(rot) {}

    void DeSerialize(const Json& j) override {
        if (j.has("position")) {
            Json posJson = j.getObject("position");
            position.x = posJson.get<float>("x", 0.0f);
            position.y = posJson.get<float>("y", 0.0f);
        }
        if (j.has("scale")) {
            Json scaleJson = j.getObject("scale");
            scale.x = scaleJson.get<float>("x", 1.0f);
            scale.y = scaleJson.get<float>("y", 1.0f);
        }
        rotation = j.get<float>("rotation", 0.0f);
    }
};