#pragma once
#include "component/Factory.h"
#include <string>
#include <typeindex>

#include <iostream>
#include <string>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include "component/Transform.h"

struct GameObject {
    uint64_t id = 0;
    std::string name;
    std::string tag;

    Transform transform;

    std::unordered_map<std::type_index, std::unique_ptr<Component>> components;

    GameObject() = default;

    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;

    GameObject(GameObject&&) = default;
    GameObject& operator=(GameObject&&) = default;

    template<typename T, typename... Args>
    T& addComponent(Args&&... args) {
        static_assert(!std::is_same_v<T, Transform>,
            "Transform is mandatory and cannot be added manually");

        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        component->owner = this;

        T* ptr = component.get();
        components[typeid(T)] = std::move(component);

        ptr->OnCreate();
        return *ptr;
    }

    template<typename T>
    T* getComponent() {
        if constexpr (std::is_same_v<T, Transform>) {
            return &transform;
        }

        auto it = components.find(typeid(T));
        if (it == components.end())
            return nullptr;

        return static_cast<T*>(it->second.get());
    }

    void addComponent(std::unique_ptr<Component> comp) {
        if (!comp) return;
        if (typeid(*comp) == typeid(Transform)) return;

        comp->owner = this;

        const auto type = std::type_index(typeid(*comp));
        components[type] = std::move(comp);

        components[type]->OnCreate();
    }

    void Update(const float dt) {
        for (auto& [_, comp] : components)
            comp->Update(dt);
    }
};

struct Camera {
    Transform transform;
    float zoom = 1.0f;
};

inline Camera camera;