#pragma once

#include <vector>
#include "GameObject.h"

struct Scene {
    std::vector<std::unique_ptr<GameObject>> objects;
    Camera camera;

    Scene() = default;

    uint32_t nextId = 1;

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    Scene(Scene&&) = default;
    Scene& operator=(Scene&&) = default;

    GameObject* createObject(const std::string& name);
    void onEnter();
    void onExit();
};