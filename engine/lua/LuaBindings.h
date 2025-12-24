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