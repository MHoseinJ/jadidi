#include "Collider.h"
#include "scene/GameObject.h"
#include "component/Rigidbody.h"
#include "core/Engine.h"
#include <cmath>

void BoxCollider::OnCreate() {
    Rigidbody* rb = owner->getComponent<Rigidbody>();
    if (!rb && size.x > 0.0f && size.y > 0.0f) {
        object = physics->createBody(
            BodyType::Static,
            owner->transform.getWorldPosition(),
            size,
            1.0f,
            1.0f,
            true,
            isTrigger,
            GameObjectHandle(owner->id)
        );
        ownsPhysicsBody = true;
        lastPosition = owner->transform.getWorldPosition();
    }
}

void BoxCollider::SyncToPhysics() {
    if (!ownsPhysicsBody)
        return;
    physics->setPosition(
        &object,
        owner->transform.getWorldPosition()
    );
}

void BoxCollider::rebuildBody() {
    if (ownsPhysicsBody) {
        physics->deleteBody(object);
        ownsPhysicsBody = false;
    }
    if (size.x > 0.0f && size.y > 0.0f) {
        Rigidbody* rb = owner->getComponent<Rigidbody>();
        if (!rb) {
            object = physics->createBody(
                BodyType::Static,
                owner->transform.getWorldPosition(),
                size,
                1.0f, 1.0f,
                true,
                isTrigger,
                GameObjectHandle(owner->id)
            );
            ownsPhysicsBody = true;
        }
    }
}

void BoxCollider::Update(float) {
    if (!ownsPhysicsBody)
        return;
    const Vector2 position = owner->transform.getWorldPosition();
    if (position != lastPosition) {
        physics->setPosition(&object, position);
        lastPosition = position;
    }
}

void BoxCollider::OnDestroy() {
    if (ownsPhysicsBody) {
        physics->deleteBody(object);
    }
}

void BoxCollider::DeSerialize(const Json& j) {
    size.x = j.get<float>("x", 0.0f);
    size.y = j.get<float>("y", 0.0f);
    isTrigger = j.get<bool>("isTrigger", false);
}

bool IsColliding(const BoxCollider* a, const BoxCollider* b) {
    const Vector2 aPosition = a->owner->transform.getWorldPosition();
    const Vector2 bPosition = b->owner->transform.getWorldPosition();
    
    const float ax = aPosition.x;
    const float ay = aPosition.y;
    const float bx = bPosition.x;
    const float by = bPosition.y;
    
    const float dx = std::abs(ax - bx);
    const float dy = std::abs(ay - by);
    const float halfWidthA = a->size.x / 2.0f;
    const float halfHeightA = a->size.y / 2.0f;
    const float halfWidthB = b->size.x / 2.0f;
    const float halfHeightB = b->size.y / 2.0f;
    return (dx < (halfWidthA + halfWidthB)) &&
           (dy < (halfHeightA + halfHeightB));
}

bool IsColliding(const Vector2* a, const BoxCollider* b) {
    const Vector2 bPosition = b->owner->transform.getWorldPosition();
    
    const float bx = bPosition.x;
    const float by = bPosition.y;
    
    const float bw = b->size.x / 2.0f;
    const float bh = b->size.y / 2.0f;
    const float dx = std::abs(a->x - bx);
    const float dy = std::abs(a->y - by);
    return (dx <= bw && dy <= bh);
}