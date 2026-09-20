#pragma once

#include <cmath>
#include "vector.h"

struct Matrix3 {
    float m[3][3];

    Matrix3();

    Matrix3(
        float m00, float m01, float m02,
        float m10, float m11, float m12,
        float m20, float m21, float m22
    );

    static Matrix3 identity();

    static Matrix3 translation(const Vector2& position);

    static Matrix3 rotation(float degrees);

    static Matrix3 scale(const Vector2& scale);

    static Matrix3 trs(
        const Vector2& position,
        float rotationDegrees,
        const Vector2& scale
    );

    Matrix3 operator*(const Matrix3& other) const;

    Vector2 transformPoint(const Vector2& point) const;

    Vector2 transformVector(const Vector2& vector) const;

    Matrix3 inverse() const;

    float determinant() const;
};