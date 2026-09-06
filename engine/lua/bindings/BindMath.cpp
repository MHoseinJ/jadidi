#include "lua/LuaBindings.h"
#include "utils/math/vector.h"
#include <SDL2/SDL.h>

void LuaBindings::bindMath(sol::state& lua) {
    lua.new_usertype<SDL_Rect>("SDL_Rect",
        "x", &SDL_Rect::x,
        "y", &SDL_Rect::y,
        "w", &SDL_Rect::w,
        "h", &SDL_Rect::h
    );

    lua.new_usertype<Vector2>("Vector2",
        sol::constructors<Vector2(), Vector2(float, float)>(),
        "x", &Vector2::x,
        "y", &Vector2::y,
        "set", &Vector2::set,
        "move", &Vector2::move,
        "distance", &Vector2::distance,
        "length", &Vector2::length,
        "normalized", &Vector2::normalized,
        "dot", &Vector2::dot,
        sol::meta_function::addition, &Vector2::operator+,
        sol::meta_function::subtraction, &Vector2::operator-,
        sol::meta_function::multiplication, &Vector2::operator*,
        sol::meta_function::division, &Vector2::operator/
    );

    lua.new_usertype<SDL_Color>("Color",
        sol::constructors<SDL_Color()>(),
        "r", &SDL_Color::r,
        "g", &SDL_Color::g,
        "b", &SDL_Color::b,
        "a", &SDL_Color::a
    );
}