#pragma once
#include <string>

#include "component/Sprite.h"
#include "component/Transform.h"

struct GameObject {
    std::string name;

    Transform transform;
    Sprite sprite;
};

struct Camera {
    Transform transform{0,0,1,1};
    float zoom = 1.0f;
};
