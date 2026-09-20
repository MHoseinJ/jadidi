#include "Transform.h"

void Transform::updateWorldMatrix() const
{
    if (!dirty)
        return;

    worldMatrix = Matrix3::trs(
        localPosition,
        localRotation,
        localScale
    );

    dirty = false;
}

void Transform::markDirty()
{
    dirty = true;
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
    // TODO: add parent then do these stuff
    return localScale;
}

float Transform::getWorldRotation() const
{
    updateWorldMatrix();
    // TODO: add parent then do these stuff
    return localRotation;
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