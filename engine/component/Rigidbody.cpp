#include "Rigidbody.h"
#include "component/Collider.h"
#include "core/Engine.h"
#include "physics/Wrapper.h"
#include "scene/GameObject.h"
#include "utils/math/vector.h"

void Rigidbody::OnCreate() {
    transform = owner->getComponent<Transform>();
    collider = owner->getComponent<BoxCollider>();

    object = physics->createBody(isDynamic ? BodyType::Dynamic : BodyType::Static, transform->position,
                                 collider ? collider->size : Vector2{0, 0}, density, friction, collider ? true : false);

    if (velocity.x != 0.0f || velocity.y != 0.0f) {
        physics->setVelocity(&object, velocity);
    }
}

void Rigidbody::Update(const float) {
    transform->position.set(physics->getPosition(&object));
    velocity = physics->getVelocity(&object);
}