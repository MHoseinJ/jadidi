#pragma once
#include <string>

#include "component/Sprite.h"
#include "component/Transform.h"

struct GameObject {
    uint64_t id;
    std::string name;
    std::string tag;

    Transform transform;
    Sprite sprite;
};

struct Camera {
    Transform transform;
    float zoom = 1.0f;
};

inline Camera camera;