#pragma once

#include "utils/Config.h"
#include "utils/math/vector.h"
#include "utils/math/matrix.h"
#include "Component.h"

struct Transform final : Component {
private:
    Vector2 localPosition;
    Vector2 localScale;
    float localRotation = 0.0f;

    mutable Matrix3 worldMatrix;
    mutable bool dirty = true;

    void updateWorldMatrix() const;

public:
    Transform()
        : localPosition(),
          localScale(1.0f, 1.0f),
          localRotation(0.0f),
          worldMatrix(Matrix3::identity()),
          dirty(true)
    {}

    Transform(
        float px,
        float py,
        float sx,
        float sy,
        float rot = 0.0f
    )
        : localPosition(px, py),
          localScale(sx, sy),
          localRotation(rot),
          worldMatrix(Matrix3::identity()),
          dirty(true)
    {}

    const Vector2& getLocalPosition() const {
        return localPosition;
    }

    const Vector2& getLocalScale() const {
        return localScale;
    }

    float getLocalRotation() const {
        return localRotation;
    }
    

    void setLocalPosition(const Vector2& position);
    void setLocalPosition(float x, float y);
    void setLocalScale(const Vector2& scale);
    void setLocalScale(float x, float y);
    void setLocalRotation(float rotation);

    void setWorldPosition(const Vector2& position);

    Vector2 getWorldPosition() const;
    Vector2 getWorldScale() const;
    float getWorldRotation() const;
    const Matrix3& getWorldMatrix() const;

    void markDirty();

    bool isDirty() const {
        return dirty;
    }

    void DeSerialize(const Json& j) override;

    std::string typeName() const override {
        return "transform";
    }
};

// helpers

struct PositionProxy {
    Transform* transform;

    float getX() const { return transform->getLocalPosition().x; }
    float getY() const { return transform->getLocalPosition().y; }

    void setX(float v) {
        Vector2 p = transform->getLocalPosition();
        p.x = v;
        transform->setLocalPosition(p);
    }

    void setY(float v) {
        Vector2 p = transform->getLocalPosition();
        p.y = v;
        transform->setLocalPosition(p);
    }

    void set(float x, float y) {
        transform->setLocalPosition(x, y);
    }

    void set(const Vector2& v) {
        transform->setLocalPosition(v);
    }

    void move(float dx, float dy) {
        Vector2 p = transform->getLocalPosition();
        p.move({dx, dy});
        transform->setLocalPosition(p);
    }

    void move(const Vector2& diff) {
        Vector2 p = transform->getLocalPosition();
        p.move(diff);
        transform->setLocalPosition(p);
    }

    Vector2 add(const Vector2& other) const {
        return transform->getLocalPosition() + other;
    }
    Vector2 sub(const Vector2& other) const {
        return transform->getLocalPosition() - other;
    }
    Vector2 mul(float scalar) const {
        return transform->getLocalPosition() * scalar;
    }
    Vector2 div(float scalar) const {
        return transform->getLocalPosition() / scalar;
    }
};

struct ScaleProxy {
    Transform* transform;

    float getX() const { return transform->getLocalScale().x; }
    float getY() const { return transform->getLocalScale().y; }

    void setX(float v) {
        Vector2 s = transform->getLocalScale();
        s.x = v;
        transform->setLocalScale(s);
    }

    void setY(float v) {
        Vector2 s = transform->getLocalScale();
        s.y = v;
        transform->setLocalScale(s);
    }

    void set(float x, float y) {
        transform->setLocalScale(x, y);
    }

    void set(const Vector2& v) {
        transform->setLocalScale(v);
    }

    void move(float dx, float dy) {
        Vector2 s = transform->getLocalScale();
        s.move({dx, dy});
        transform->setLocalScale(s);
    }

    void move(const Vector2& diff) {
        Vector2 s = transform->getLocalScale();
        s.move(diff);
        transform->setLocalScale(s);
    }

    Vector2 add(const Vector2& other) const {
        return transform->getLocalScale() + other;
    }
    Vector2 sub(const Vector2& other) const {
        return transform->getLocalScale() - other;
    }
    Vector2 mul(float scalar) const {
        return transform->getLocalScale() * scalar;
    }
    Vector2 div(float scalar) const {
        return transform->getLocalScale() / scalar;
    }
};