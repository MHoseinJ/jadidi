
#pragma once

#include <stdexcept>
#include <climits>
#include <functional>
#include <memory>
#include <utility>
#include <unordered_map>
#include <string>
#include <string_view>

#include <sol/sol.hpp>

#include "Animation.h"
#include "Animator.h"
#include "Component.h"
#include "Sprite.h"
#include "Transform.h"

int levenshtein(const std::string& a, const std::string& b);

struct Animator;

class Factory {
public:
    using Creator = std::function<std::unique_ptr<Component>()>;

    using LuaPusher =
        std::function<sol::object(sol::state_view, Component*)>;

    static Factory& instance() {
        static Factory inst;
        return inst;
    }

    // Register a component with its concrete Lua userdata type.
    template<typename T>
    void registerComponent(std::string_view name, Creator fn) {
        registerComponent(
            name,
            std::move(fn),
            [](sol::state_view lua, Component* component) {
                return sol::make_object(lua, static_cast<T*>(component));
            }
        );
    }

    // Internal registration overload.
    void registerComponent(
        std::string_view name,
        Creator fn,
        LuaPusher pusher
    );

    std::unique_ptr<Component> create(const std::string& name) const;

    sol::object toLua(
        sol::state_view lua,
        Component* component
    ) const;

private:
    std::unordered_map<std::string, Creator> creators;
    std::unordered_map<std::string, LuaPusher> luaPushers;
};

void registerComponents();