#pragma once
#include <sol/function.hpp>
#include "Component.h"

struct Button final : Component {
    int zOrder;

    void addFunction(const sol::function& function, int mouse);
    void callFunction(int mouseKey);
    void DeSerialize(const Json& j) override;
    nlohmann::json Serialize() const override {
        return {
            {"zOrder", zOrder}
        };
    }
    void OnCreate() override;

    std::string typeName() const override { return "button"; }

private:
    std::unordered_map<int, sol::function> functions;
};