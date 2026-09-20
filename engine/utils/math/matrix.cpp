#include "matrix.h"

Matrix3::Matrix3()
    : m{
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f}
    }
{
}

Matrix3::Matrix3(
    float m00, float m01, float m02,
    float m10, float m11, float m12,
    float m20, float m21, float m22
)
    : m{
        {m00, m01, m02},
        {m10, m11, m12},
        {m20, m21, m22}
    }
{
}

Matrix3 Matrix3::identity()
{
    return Matrix3();
}

Matrix3 Matrix3::translation(const Vector2& position)
{
    return Matrix3(
        1.0f, 0.0f, position.x,
        0.0f, 1.0f, position.y,
        0.0f, 0.0f, 1.0f
    );
}

Matrix3 Matrix3::rotation(float degrees)
{
    constexpr float pi = 3.14159265358979323846f;

    const float radians = degrees * pi / 180.0f;
    const float c = std::cos(radians);
    const float s = std::sin(radians);

    return Matrix3(
         c, -s, 0.0f,
         s,  c, 0.0f,
        0.0f, 0.0f, 1.0f
    );
}

Matrix3 Matrix3::scale(const Vector2& scale)
{
    return Matrix3(
        scale.x, 0.0f,    0.0f,
        0.0f,    scale.y, 0.0f,
        0.0f,    0.0f,    1.0f
    );
}

Matrix3 Matrix3::trs(
    const Vector2& position,
    float rotationDegrees,
    const Vector2& scaleValue
)
{
    return translation(position)
         * rotation(rotationDegrees)
         * scale(scaleValue);
}

Matrix3 Matrix3::operator*(const Matrix3& other) const
{
    Matrix3 result(
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f
    );

    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            for (int i = 0; i < 3; ++i) {
                result.m[row][col] += m[row][i] * other.m[i][col];
            }
        }
    }

    return result;
}

Vector2 Matrix3::transformPoint(const Vector2& point) const
{
    return Vector2{
        m[0][0] * point.x + m[0][1] * point.y + m[0][2],
        m[1][0] * point.x + m[1][1] * point.y + m[1][2]
    };
}

Vector2 Matrix3::transformVector(const Vector2& vector) const
{
    return Vector2{
        m[0][0] * vector.x + m[0][1] * vector.y,
        m[1][0] * vector.x + m[1][1] * vector.y
    };
}

float Matrix3::determinant() const
{
    return
        m[0][0] * (
            m[1][1] * m[2][2] -
            m[1][2] * m[2][1]
        )
        -
        m[0][1] * (
            m[1][0] * m[2][2] -
            m[1][2] * m[2][0]
        )
        +
        m[0][2] * (
            m[1][0] * m[2][1] -
            m[1][1] * m[2][0]
        );
}

Matrix3 Matrix3::inverse() const
{
    const float det = determinant();

    constexpr float epsilon = 0.000001f;

    if (std::abs(det) < epsilon) {
        return Matrix3::identity();
    }

    const float invDet = 1.0f / det;

    Matrix3 result;

    result.m[0][0] =
        (m[1][1] * m[2][2] - m[1][2] * m[2][1]) * invDet;

    result.m[0][1] =
        (m[0][2] * m[2][1] - m[0][1] * m[2][2]) * invDet;

    result.m[0][2] =
        (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * invDet;


    result.m[1][0] =
        (m[1][2] * m[2][0] - m[1][0] * m[2][2]) * invDet;

    result.m[1][1] =
        (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * invDet;

    result.m[1][2] =
        (m[0][2] * m[1][0] - m[0][0] * m[1][2]) * invDet;


    result.m[2][0] =
        (m[1][0] * m[2][1] - m[1][1] * m[2][0]) * invDet;

    result.m[2][1] =
        (m[0][1] * m[2][0] - m[0][0] * m[2][1]) * invDet;

    result.m[2][2] =
        (m[0][0] * m[1][1] - m[0][1] * m[1][0]) * invDet;

    return result;
}