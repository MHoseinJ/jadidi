#pragma once
#include "Component.h"
#include "utils/math/vector.h"
#include "physics/Wrapper.h"

struct BoxCollider final : Component {
    Vector2 size;
    Object object;
    bool ownsPhysicsBody = false;

    void OnCreate() override;

    void rebuildBody();
    
    void OnDestroy() override;
    void DeSerialize(const json& j) override;
};

bool IsColliding(const BoxCollider* a, const BoxCollider* b);
bool IsColliding(const Vector2* a, const BoxCollider* b);