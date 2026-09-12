#include "Rigidbody.h"
#include "component/Collider.h"
#include "core/Engine.h"
#include "lua/GameObjectHandle.h"
#include "physics/Wrapper.h"
#include "scene/GameObject.h"
#include "utils/math/vector.h"

void Rigidbody::OnCreate() {
    transform = owner->getComponent<Transform>();
    collider = owner->getComponent<BoxCollider>();

    // If the collider created its own static body before the
    // Rigidbody existed, remove that temporary body.
    if (collider && collider->ownsPhysicsBody) {
        physics->deleteBody(collider->object);
        collider->ownsPhysicsBody = false;
    }

    const bool hasCollider = (collider != nullptr);
    const bool isTrig = hasCollider ? collider->isTrigger : false;

    object = physics->createBody(
        bodyType,
        transform->position,
        hasCollider ? collider->size : Vector2{0, 0},
        density,
        friction,
        hasCollider,
        isTrig,
        GameObjectHandle(owner->id)
    );

    if (velocity.x != 0.0f || velocity.y != 0.0f) {
        physics->setVelocity(&object, velocity);
    }
}

void Rigidbody::setBodyType(BodyType type) {
    bodyType = type;
    physics->setBodyType(&object, type);
}

void Rigidbody::Update(const float) {
    transform->position.set(physics->getPosition(&object));
    velocity = physics->getVelocity(&object);
}

void Rigidbody::OnDestroy() {
    physics->deleteBody(object);
}

void Rigidbody::setDensity(float value) {
    density = value;
    physics->setShapeDensity(&object, value);
}

void Rigidbody::setFriction(float value) {
    friction = value;
    physics->setShapeFriction(&object, value);
}

void Rigidbody::setVelocity(Vector2 value) {
    velocity = value;
    physics->setVelocity(&object, value);
}

void Rigidbody::applyImpulse(Vector2 impulse) {
    physics->applyImpulse(&object, impulse);
}

void Rigidbody::setPosition(Vector2 value) {
    transform->position = value;
    physics->setPosition(&object, value);
}