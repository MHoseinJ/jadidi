#pragma once
#include "utils/Config.h"

struct GameObject; // forward declaration

struct Component {
    GameObject* owner = nullptr;

    virtual ~Component() = default;

    virtual void OnCreate() {}
    virtual void OnDestroy() {}
    virtual void Update(float dt) {}
    virtual void DeSerialize(const json& j) = 0;
};