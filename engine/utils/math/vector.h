#pragma once

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

    Vector2 operator+(const Vector2& vecToAdd) const {
        return {x + vecToAdd.x, y + vecToAdd.y};
    }

    Vector2 operator*(const float scalar) const {
        return {x * scalar, y * scalar};
    }
};
