#pragma once
#include <sol/sol.hpp>

#include "LuaObject.h"

extern sol::state lua;
extern std::vector<LuaObject> scripts;

namespace Lua {
    void init();
    void loadSceneScripts(const std::string& sceneName);
    void callStartLua();
    void callUpdateLua(float dt);
    void bindFunctions();
}

namespace LuaBindings {
    void bindCore(sol::state& lua);
    void bindMath(sol::state& lua);
    void bindInput(sol::state& lua);
    void bindScene(sol::state& lua);
    void bindDebug(sol::state& lua);
}