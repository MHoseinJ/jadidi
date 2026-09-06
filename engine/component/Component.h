#pragma once
#include "utils/Config.h"
#include "lua/GameObjectHandle.h"

struct GameObject; // forward declaration

struct Component {
    GameObject* owner = nullptr;

    virtual ~Component() = default;

    virtual void OnCreate() {}
    virtual void OnDestroy() {}
    virtual void Update(float dt) {}
    virtual void DeSerialize(const json& j) = 0;

    virtual void OnCollisionEnter(GameObjectHandle other) {}
    virtual void OnCollisionExit(GameObjectHandle other) {}
    virtual void OnTriggerEnter(GameObjectHandle other) {}
    virtual void OnTriggerExit(GameObjectHandle other) {}
};