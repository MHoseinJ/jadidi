#include "core/Input.h"
#include "lua/LuaApi.h"
#include "lua/LuaBindings.h"

void LuaBindings::bindInput(sol::state& lua) {
    LuaApi::bindKeys(lua);
    LuaApi::bindMouse(lua);

    auto input = lua["Input"].get_or_create<sol::table>();

    input.set_function("keyPressed", static_cast<bool (*)(int)>(&LuaApi::isKeyPressed));
    input.set_function("keyDown", static_cast<bool (*)(int)>(&LuaApi::isKeyDown));
    input.set_function("keyUp", static_cast<bool (*)(int)>(&LuaApi::isKeyUp));

    input.set_function("mousePressed", static_cast<bool (*)(int)>(&LuaApi::isMousePressed));
    input.set_function("mouseDown", static_cast<bool (*)(int)>(&LuaApi::isMouseDown));
    input.set_function("mouseUp", static_cast<bool (*)(int)>(&LuaApi::isMouseUp));

    auto mouse = lua["Mouse"].get_or_create<sol::table>();
    mouse.set_function("position", &Input::GetMousePositionVec);
    mouse.set_function("world_position", &Input::GetMouseWorldPos);
}