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

void Rigidbody::DeSerialize(const Json& j) {
    if (j.has("velocity")) {
        Json velocityJson = j.getObject("velocity");
        velocity.x = velocityJson.get<float>("x", 0.0f);
        velocity.y = velocityJson.get<float>("y", 0.0f);
    }
    density = j.get<float>("density", 1.0f);
    friction = j.get<float>("friction", 1.0f);
    
    std::string type = j.get<std::string>("bodyType", "static");
    if (type == "dynamic") bodyType = BodyType::Dynamic;
    else if (type == "kinematic") bodyType = BodyType::Kinematic;
    else bodyType = BodyType::Static;
}