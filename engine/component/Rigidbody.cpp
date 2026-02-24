#include "Rigidbody.h"

#include "scene/GameObject.h"

void Rigidbody::OnCreate() {
    transform = owner->getComponent<Transform>();
}

void Rigidbody::Update(const float deltaTime) {
    transform->position.move(velocity * deltaTime);
}

