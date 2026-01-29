#pragma once

struct Vector2 {
    float x;
    float y;

    void move(const Vector2 &diff) {
        this->x += diff.x;
        this->y += diff.y;
    }


    void set(const Vector2 &pos) {
        this->x = pos.x;
        this->y = pos.y;
    }
};
