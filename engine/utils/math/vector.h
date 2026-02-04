#pragma once

struct Vector2 {
    float x;
    float y;

    Vector2() : x(0), y(0) {}
    Vector2(float x_, float y_) : x(x_), y(y_) {}

    void move(const Vector2& diff) {
        x += diff.x;
        y += diff.y;
    }

    void set(const Vector2& pos) {
        x = pos.x;
        y = pos.y;
    }
};
