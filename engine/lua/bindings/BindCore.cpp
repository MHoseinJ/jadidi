#include "lua/LuaBindings.h"

void LuaBindings::bindCore(sol::state& lua) {
    lua.open_libraries(
        sol::lib::base,
        sol::lib::math,
        sol::lib::table,
        sol::lib::string
    );
}