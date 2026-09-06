#pragma once
#include <math.h>

struct Vector2 {
    float x;
    float y;

    Vector2() : x(0), y(0) {}
    Vector2(const float x_, const float y_) : x(x_), y(y_) {}

    void move(const Vector2& diff) {
        x += diff.x;
        y += diff.y;
    }

    void set(const Vector2& pos) {
        x = pos.x;
        y = pos.y;
    }

    [[nodiscard]] float distance(const Vector2& pos) const {
        return sqrtf(powf(pos.x - x, 2) + powf(pos.y - y, 2));
    }

    [[nodiscard]] float length() const {
        return sqrtf(x * x + y * y);
    }

    [[nodiscard]] Vector2 normalized() const {
        float len = length();
        if (len == 0.0f) return {0, 0};
        return {x / len, y / len};
    }

    [[nodiscard]] float dot(const Vector2& other) const {
        return x * other.x + y * other.y;
    }

    Vector2 operator+(const Vector2& other) const {
        return {x + other.x, y + other.y};
    }

    Vector2 operator-(const Vector2& other) const {
        return {x - other.x, y - other.y};
    }

    Vector2 operator*(const float scalar) const {
        return {x * scalar, y * scalar};
    }

    Vector2 operator/(const float scalar) const {
        if (scalar == 0.0f) return {0, 0};
        return {x / scalar, y / scalar};
    }
};