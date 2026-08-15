#pragma once

#include <cstdint>

struct GameObject;

struct GameObjectHandle {
    uint64_t id = 0;

    mutable bool invalidLogged = false;

    GameObjectHandle() = default;
    explicit GameObjectHandle(uint64_t id);

    bool isValid() const;

    GameObject* resolve() const;

    GameObject* resolveOrLog() const;

    bool operator==(const GameObjectHandle& other) const;
};