#pragma once
#include "Component.h"
#include "Transform.h"
#include "component/Collider.h"
#include "physics/Wrapper.h"

struct Rigidbody final : Component {
    Transform* transform = nullptr;
    BoxCollider* collider = nullptr;
    Vector2 velocity = {0.0f, 0.0f};
    float density = 1.0f;
    float friction = 1.0f;
    bool isDynamic = false;
    Object object;

    void OnCreate() override;
    void Update(float deltaTime) override;

    void DeSerialize(const json& j) override {
        if (j.contains("velocity")) {
            velocity.x = j["velocity"].value("x", 0.0f);
            velocity.y = j["velocity"].value("y", 0.0f);
        }
        if (j.contains("density")) {
            density = j.value("density", 1.0f);
        }
        if (j.contains("friction")) {
            friction = j.value("friction", 1.0f);
        }
        if (j.contains("isDynamic")) {
            isDynamic = j.value("isDynamic", false);
        }
    }
};