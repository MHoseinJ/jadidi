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

void Button::DeSerialize(const Json& j) {
    if (j.has("functions")) {
        Json functionsArray = j.getObject("functions");
        for (size_t i = 0; i < functionsArray.size(); ++i) {
            Json it = functionsArray[i];
            if (it.has("name") && it.has("mouse")) {
                std::string name = it.get<std::string>("name", "");
                const int mouse = it.get<int>("mouse", 0);
                if (name.empty()) {
                    gameLog("Failed to deserialize button function named \"" + name + "\"", ERROR);
                    return;
                }
                const sol::function function = LuaApi::getFunctionByName(name);
                if (function == sol::nil) {
                    gameLog("there is no function to add to button", ERROR);
                    return;
                }
                addFunction(function, mouse);
            } else {
                gameLog("Failed to deserialize function: name or mouse is empty", ERROR);
            }
        }
    }
    zOrder = j.get<int>("zOrder", 0);
}

void Button::OnCreate() {
    zOrder = 0;
}