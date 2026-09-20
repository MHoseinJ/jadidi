#pragma once

#include "component/Factory.h"

#include <string>
#include <sol/sol.hpp>
#include <iostream>
#include <unordered_map>
#include <memory>

#include "component/Transform.h"

struct GameObject {
    uint64_t id = 0;
    std::string name;
    std::string tag;

    Transform transform;

    std::unordered_map<std::string, std::unique_ptr<Component>> components;

    GameObject* parent = nullptr;
    std::vector<GameObject*> children;

    sol::function onCollisionEnterCallback;
    sol::function onCollisionExitCallback;
    sol::function onTriggerEnterCallback;
    sol::function onTriggerExitCallback;

    GameObject()
    {
        transform.owner = this;
    }

    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;

    GameObject(GameObject&&) = default;
    GameObject& operator=(GameObject&&) = default;

    bool setParent(GameObject* newParent, bool keepWorldTransform = true);
    void removeChild(GameObject* child);
    
    bool isChildOf(const GameObject* object) const;
    bool isDescendantOf(const GameObject* object) const;
    
    GameObject* getParent() const {
        return parent;
    }
    
    const std::vector<GameObject*>& getChildren() const {
        return children;
    }

    template<typename T, typename... Args>
    T& addComponent(Args&&... args) {
        static_assert(
            !std::is_same_v<T, Transform>,
            "Transform is mandatory and cannot be added manually"
        );

        auto component =
            std::make_unique<T>(std::forward<Args>(args)...);

        component->owner = this;

        T* ptr = component.get();
        const std::string type = ptr->typeName();

        // Destroy the previous component before replacing it.
        auto it = components.find(type);
        if (it != components.end()) {
            it->second->OnDestroy();
            components.erase(it);
        }

        components.emplace(type, std::move(component));

        ptr->OnCreate();
        return *ptr;
    }

    template<typename T>
    T* getComponent() {
        if constexpr (std::is_same_v<T, Transform>) {
            return &transform;
        }

        T temp;

        auto it = components.find(temp.typeName());
        if (it == components.end()) {
            return nullptr;
        }

        return static_cast<T*>(it->second.get());
    }

    template<typename T>
    const T* getComponent() const {
        if constexpr (std::is_same_v<T, Transform>) {
            return &transform;
        }

        T temp;

        auto it = components.find(temp.typeName());
        if (it == components.end()) {
            return nullptr;
        }

        return static_cast<const T*>(it->second.get());
    }

    void addComponent(std::unique_ptr<Component> comp) {
        if (!comp) {
            return;
        }

        gameLog("[DEBUG] Adding component type: " + comp->typeName(), INFO);

        if (typeid(*comp) == typeid(Transform)) {
            return;
        }

        comp->owner = this;

        const std::string type = comp->typeName();

        // Destroy the previous component before replacing it.
        auto it = components.find(type);
        if (it != components.end()) {
            it->second->OnDestroy();
            components.erase(it);
        }

        components.emplace(type, std::move(comp));

        components[type]->OnCreate();
    }

    void Update(const float dt) {
        for (auto& [_, comp] : components) {
            comp->Update(dt);
        }
    }
};

struct Camera {
    Transform transform;
    float zoom = 1.0f;
};

inline Camera camera;