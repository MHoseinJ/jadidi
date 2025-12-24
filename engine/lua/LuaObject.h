#pragma once
#include <sol/sol.hpp>

struct LuaObject {
    sol::environment env;
    sol::function start;
    sol::function update;
};
