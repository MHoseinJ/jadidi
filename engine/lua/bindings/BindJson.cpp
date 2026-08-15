#include "lua/LuaBindings.h"
#include "lua/LuaApi.h"
#include "utils/FileSystem.h"

void LuaBindings::bindJson(sol::state& lua) {
    auto json = lua["Json"].get_or_create<sol::table>();
    
    json.set_function("read", [&](const std::string& path) -> sol::object {
        if (!fs::fileExists(path))
            return sol::nil;
        auto j = fs::readJson(path);
        return LuaApi::LuaJSON(j);
    });
    
    json.set_function("write", [&](const std::string& path, const sol::object& table) -> bool {
        if (table.get_type() != sol::type::table)
            return false;
        auto j = LuaApi::LuaJSON(table);
        return fs::writeJson(path, j);
    });
}