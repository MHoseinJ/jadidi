#pragma once

#include "Factory.h"

#include <functional>
#include <memory>
#include <utility>

#include "Animation.h"
#include "Animator.h"
#include "Component.h"
#include "Sprite.h"
#include "Transform.h"

struct Animator;

class Factory {
public:
    using Creator = std::function<std::unique_ptr<Component>()>;

    static Factory& instance() {
        static Factory inst;
        return inst;
    }

    void registerComponent(std::string_view name, Creator fn);

    std::unique_ptr<Component> create(const std::string& name) const {
        auto it = creators.find(name);

        if (it == creators.end())
            return nullptr;

        return it->second();
    }

private:
    std::unordered_map<std::string, Creator> creators;
};

void registerComponents();