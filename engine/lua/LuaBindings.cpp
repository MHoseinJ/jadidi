#include "LuaBindings.h"

sol::state lua;
std::string pendingSceneLoad = "";

void Lua::init() {
    LuaBindings::bindCore(lua);
    LuaBindings::bindMath(lua);
    LuaBindings::bindInput(lua);
    LuaBindings::bindDebug(lua);
    LuaBindings::bindAsset(lua);
    LuaBindings::bindState(lua);
    LuaBindings::bindECS(lua);
    LuaBindings::bindScene(lua);
}