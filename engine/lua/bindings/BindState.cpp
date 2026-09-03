#include "core/State.h"
#include "lua/LuaBindings.h"

void LuaBindings::bindState(sol::state& lua) {
    auto state = lua["State"].get_or_create<sol::table>();

    state.set_function("set",
                       [](const std::string& key, const sol::object& value) { State::instance().set(key, value); });

    state.set_function("get", [](const std::string& key) -> sol::object { return State::instance().get(key); });

    state.set_function("exists", [](const std::string& key) { return State::instance().exists(key); });

    state.set_function("remove", [](const std::string& key) { State::instance().remove(key); });

    state.set_function("clear", []() { State::instance().clear(); });
}