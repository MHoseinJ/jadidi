#include "Button.h"

#include "core/Log.h"
#include "lua/LuaApi.h"
#include "lua/LuaBindings.h"

void Button::callFunction(const int mouseKey) {
    const auto it = functions.find(mouseKey - 1);
    if (it == functions.end()) {
        return;
    }

    const sol::protected_function function = it->second;
    if (const auto result = function(); !result.valid()) {
        const sol::error err = result;
        gameLog("[LUA Function] Error: " + std::string(err.what()), ERROR);
    }
}

void Button::addFunction(const sol::function& function, const int mouse) {
    functions[mouse] = function;
}

void Button::DeSerialize(const json &j) {
    if (j.contains("functions")) {
        for (const auto& it : j["functions"]) {
            if (it.contains("name") && it.contains("mouse")) {
                if (it["name"].is_string() && it["mouse"].is_number_integer()) {
                    std::string name = it.value("name", "");
                    const int mouse = it.value("mouse", 0);

                    if (name.empty()) {
                        gameLog("Failed to deserialize button function named \"" + name + "\"", ERROR);
                        return;
                    }

                    const sol::function function = LuaApi::getFunctionByName(name);
                    if ( function == sol::nil ) {
                        gameLog("there is no function to add to button", ERROR);
                        return;
                    }
                    addFunction(function, mouse);
                } else gameLog("Failed to deserialize function " + to_string(it["name"]), ERROR);
            } else gameLog("Failed to deserialize function: name or mouse is empty" + to_string(it["name"]), ERROR);
        }
    }
    if (j.contains("zOrder")) {
        zOrder = j["zOrder"].get<int>();
    }
}

void Button::OnCreate() {
    zOrder = 0;
}
