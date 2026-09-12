#pragma once
#include "Component.h"
#include "utils/math/vector.h"
#include "physics/Wrapper.h"

struct BoxCollider final : Component {
    Vector2 size;
    Object object;
    bool ownsPhysicsBody = false;
    bool isTrigger = false;
    Vector2 lastPosition;

    void OnCreate() override;
    void Update(float deltaTime) override;
    void OnDestroy() override;
    void DeSerialize(const Json& j) override;
    void rebuildBody();
    void SyncToPhysics();
};

bool IsColliding(const BoxCollider* a, const BoxCollider* b);
bool IsColliding(const Vector2* a, const BoxCollider* b);