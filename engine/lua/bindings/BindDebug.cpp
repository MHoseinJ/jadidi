#include "lua/LuaBindings.h"
#include "lua/LuaApi.h"

void LuaBindings::bindDebug(sol::state& lua) {
    auto log = lua["Log"].get_or_create<sol::table>();
    
    log.set_function("clear", &LuaApi::clear);
    log.set_function("print", &LuaApi::print);
    log.set_function("info",  &LuaApi::info);
    log.set_function("warn",  &LuaApi::warn);
    log.set_function("error", &LuaApi::error);
}