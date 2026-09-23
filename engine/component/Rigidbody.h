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
    BodyType bodyType = BodyType::Static;
    Object object;

    void OnCreate() override;
    void Update(float deltaTime) override;
    void OnDestroy() override;
    void setBodyType(BodyType type);
    void setDensity(float value);
    void setFriction(float value);
    void setVelocity(Vector2 value);
    void setPosition(Vector2 value);
    void applyImpulse(Vector2 impulse);
    void DeSerialize(const Json& j) override;
    nlohmann::json Serialize() const override {
        std::string type = "static";
    
        if (bodyType == BodyType::Dynamic)
            type = "dynamic";
        else if (bodyType == BodyType::Kinematic)
            type = "kinematic";
    
        return {
            {"velocity", {
                {"x", velocity.x},
                {"y", velocity.y}
            }},
            {"density", density},
            {"friction", friction},
            {"bodyType", type}
        };
    }

    std::string typeName() const override { return "rigidbody"; }
};