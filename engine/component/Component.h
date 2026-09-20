#pragma once
#include "utils/Json.h"
#include "lua/GameObjectHandle.h"
#include <string>


struct GameObject; // forward declaration

struct Component {
    GameObject* owner = nullptr;
    
    virtual ~Component() = default;

    virtual void awake() {}
    virtual void OnCreate() {}
    virtual void OnDestroy() {}
    virtual void Update(float dt) {}
    virtual void DeSerialize(const Json& j) = 0;
    
    virtual void OnCollisionEnter(GameObjectHandle other) {}
    virtual void OnCollisionExit(GameObjectHandle other) {}
    virtual void OnTriggerEnter(GameObjectHandle other) {}
    virtual void OnTriggerExit(GameObjectHandle other) {}
    
    virtual std::string typeName() const { return ""; }
};