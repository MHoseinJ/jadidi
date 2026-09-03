#pragma once
#include "Component.h"
#include "Transform.h"
#include "component/Collider.h"
#include "physics/Wrapper.h"

struct Rigidbody final : Component {

    Transform* transform;
    BoxCollider* collider;
    Vector2 velocity;
    float density;
    float friction;
    bool isDynamic;
    Object object;

    void OnCreate() override;
    void Update(float deltaTime) override;

    void DeSerialize(const json &j) override {
        if (j.contains("velocity")) {
            velocity.x = j["velocity"].value("x", 0);
            velocity.y = j["velocity"].value("y", 0);
        }
        if (j.contains("density")) {
            density = j.value("density", 1);
        }
        if (j.contains("friction")) {
            friction = j.value("friction", 1);
        }
        if (j.contains("isDynamic")) {
            isDynamic = j.value("isDynamic", false);
        }
    };
    
};