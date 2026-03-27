#pragma once
#include <sol/function.hpp>

#include "Component.h"

struct Button final : Component {

    void addFunction(const sol::function& function, int mouse);
    void callFunction(int mouseKey);
    void DeSerialize(const json &j) override;

private:
    std::unordered_map<int, sol::function> functions;
};