#pragma once
#include "utils/math/vector.h"

struct Transform {
    Vector2 position;
    Vector2 scale;

    Transform() : position(), scale(1.0f, 1.0f) {}
    Transform(float px, float py, float sx, float sy)
        : position(px, py), scale(sx, sy) {}
};