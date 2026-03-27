#pragma once
#include "Component.h"
#include "utils/math/vector.h"

struct BoxCollider final : Component {
    Vector2 size;

    void DeSerialize(const json &j) override;
};

// helper
bool IsColliding(const BoxCollider *a, const BoxCollider *b);
bool IsColliding(const Vector2& a, const BoxCollider& b);
