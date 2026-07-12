#pragma once

#include <stdexcept>

#include <functional>
#include <memory>
#include <utility>

#include "Animation.h"
#include "Animator.h"
#include "Component.h"
#include "Sprite.h"
#include "Transform.h"

// helper

int levenshtein(const std::string& a, const std::string& b);

// helper

struct Animator;

class Factory {
public:
    using Creator = std::function<std::unique_ptr<Component>()>;

    static Factory& instance() {
        static Factory inst;
        return inst;
    }

    void registerComponent(std::string_view name, Creator fn);
    
    std::unique_ptr<Component> create(const std::string& name) const
    {
        auto it = creators.find(name);
    
        if (it != creators.end())
            return it->second();
    
        std::string suggestion;
        int bestDistance = INT_MAX;
    
        for (const auto& [componentName, _] : creators) {
            int distance = levenshtein(name, componentName);
    
            if (distance < bestDistance) {
                bestDistance = distance;
                suggestion = componentName;
            }
        }
    
        if (bestDistance <= 2) {
            throw std::runtime_error(
                "Unknown component '" + name +
                "'. Did you mean '" + suggestion + "'?"
            );
        }
    
        throw std::runtime_error(
            "Unknown component '" + name + "'"
        );
    }

private:
    std::unordered_map<std::string, Creator> creators;
};

void registerComponents();