#pragma once
#include "Component.h"
#include "utils/math/vector.h"
#include "physics/Wrapper.h"


struct BoxCollider final : Component {
    Vector2 size;
    Object object;
    bool ownsPhysicsBody = false;
    bool isTrigger = false;
    
    void OnCreate() override;
    void OnDestroy() override;
    void DeSerialize(const json& j) override;
    
    void rebuildBody();
};

bool IsColliding(const BoxCollider* a, const BoxCollider* b);
bool IsColliding(const Vector2* a, const BoxCollider* b);