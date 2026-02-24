#pragma once
#include "Component.h"
#include "Transform.h"

struct Rigidbody final : Component {

    Transform* transform;
    Vector2 velocity;

    void OnCreate() override;
    void Update(float deltaTime) override;

    void DeSerialize(const json &j) override {
        velocity.x = j["velocity"]["x"];
        velocity.y = j["velocity"]["y"];
    };

};