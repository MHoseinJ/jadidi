#pragma once
#include <sol/sol.hpp>

extern sol::state lua;

namespace Lua {
    void init();
    void bindFunctions();
} // namespace Lua

namespace LuaBindings {
    void bindCore(sol::state& lua);
    void bindMath(sol::state& lua);
    void bindInput(sol::state& lua);
    void bindScene(sol::state& lua);
    void bindDebug(sol::state& lua);
    void bindAsset(sol::state& lua);
    void bindECS(sol::state& lua);
    void bindState(sol::state& lua);
    void bindJson(sol::state& lua);
} // namespace LuaBindings