#include "Collider.h"
#include "scene/GameObject.h"
#include <cmath>

void BoxCollider::DeSerialize(const json& j) {
    if (j.contains("w")) {
        size.x = j["w"].get<float>();
    } else {
        size.x = 0;
    }
    if (j.contains("h")) {
        size.y = j["h"].get<float>();
    } else {
        size.y = 0;
    }
}

bool IsColliding(const BoxCollider* a, const BoxCollider* b) {
    const float ax = a->owner->transform.position.x;
    const float ay = a->owner->transform.position.y;
    const float bx = b->owner->transform.position.x;
    const float by = b->owner->transform.position.y;

    const float dx = std::abs(ax - bx);
    const float dy = std::abs(ay - by);

    const float halfWidthA = a->size.x / 2.0f;
    const float halfHeightA = a->size.y / 2.0f;
    const float halfWidthB = b->size.x / 2.0f;
    const float halfHeightB = b->size.y / 2.0f;

    return (dx < (halfWidthA + halfWidthB)) && (dy < (halfHeightA + halfHeightB));
}

bool IsColliding(const Vector2* a, const BoxCollider* b) {

    const float bx = b->owner->transform.position.x;
    const float by = b->owner->transform.position.y;
    const float bw = b->size.x / 2.0f;
    const float bh = b->size.y / 2.0f;

    const float dx = std::abs(a->x - bx);
    const float dy = std::abs(a->y - by);

    if (dx <= bw && dy <= bh) {
        return true;
    }
    return false;
}