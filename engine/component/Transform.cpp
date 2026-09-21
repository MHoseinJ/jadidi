#include "Transform.h"
#include "scene/GameObject.h"

void Transform::updateWorldMatrix() const
{
    if (!dirty)
        return;

    Matrix3 localMatrix = Matrix3::trs(
        localPosition,
        localRotation,
        localScale
    );

    if (owner && owner->getParent()) {
        worldMatrix =
            owner->getParent()->transform.getWorldMatrix()
            * localMatrix;
    }
    else {
        worldMatrix = localMatrix;
    }

    dirty = false;
}

void Transform::markDirty()
{
    dirty = true;

    if (!owner)
        return;

    for (GameObject* child : owner->getChildren()) {
        if (child) {
            child->transform.markDirty();
        }
    }
}

void Transform::setLocalPosition(const Vector2& position)
{
    localPosition = position;
    markDirty();
}

void Transform::setLocalPosition(float x, float y)
{
    localPosition.set(x, y);
    markDirty();
}

void Transform::setLocalScale(const Vector2& scale)
{
    localScale = scale;
    markDirty();
}

void Transform::setLocalScale(float x, float y)
{
    localScale.set(x, y);
    markDirty();
}

void Transform::setLocalRotation(float rotation)
{
    localRotation = rotation;
    markDirty();
}

Vector2 Transform::getWorldPosition() const
{
    updateWorldMatrix();

    return worldMatrix.transformPoint(Vector2{0.0f, 0.0f});
}

Vector2 Transform::getWorldScale() const
{
    updateWorldMatrix();

    float scaleX = std::sqrt(
        worldMatrix.m[0][0] * worldMatrix.m[0][0] +
        worldMatrix.m[1][0] * worldMatrix.m[1][0]
    );

    float scaleY = std::sqrt(
        worldMatrix.m[0][1] * worldMatrix.m[0][1] +
        worldMatrix.m[1][1] * worldMatrix.m[1][1]
    );

    return Vector2{scaleX, scaleY};
}

float Transform::getWorldRotation() const
{
    updateWorldMatrix();

    constexpr float pi = 3.14159265358979323846f;

    float radians = std::atan2(
        worldMatrix.m[1][0],
        worldMatrix.m[0][0]
    );

    return radians * 180.0f / pi;
}

const Matrix3& Transform::getWorldMatrix() const
{
    updateWorldMatrix();
    return worldMatrix;
}

void Transform::DeSerialize(const Json& j)
{
    if (j.has("position")) {
        Json posJson = j.getObject("position");

        setLocalPosition(
            posJson.get<float>("x", 0.0f),
            posJson.get<float>("y", 0.0f)
        );
    }

    if (j.has("scale")) {
        Json scaleJson = j.getObject("scale");

        setLocalScale(
            scaleJson.get<float>("x", 1.0f),
            scaleJson.get<float>("y", 1.0f)
        );
    }

    setLocalRotation(
        j.get<float>("rotation", 0.0f)
    );
}