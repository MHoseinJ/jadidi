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