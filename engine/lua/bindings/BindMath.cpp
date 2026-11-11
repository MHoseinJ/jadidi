#include "lua/LuaBindings.h"
#include "utils/math/vector.h"
#include <SDL2/SDL.h>
#include "component/Transform.h"

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
        
        "set", static_cast<void (Vector2::*)(float, float)>(&Vector2::set),
        "setVec", static_cast<void (Vector2::*)(const Vector2&)>(&Vector2::set),
        
        "move", &Vector2::move,
        "distance", &Vector2::distance,
        
        "length", static_cast<float (Vector2::*)() const>(&Vector2::length),
        "normalized", static_cast<Vector2 (Vector2::*)() const>(&Vector2::normalized),
        "dot", static_cast<float (Vector2::*)(const Vector2&) const>(&Vector2::dot),
        
        sol::meta_function::addition, static_cast<Vector2 (Vector2::*)(const Vector2&) const>(&Vector2::operator+),
        sol::meta_function::subtraction, static_cast<Vector2 (Vector2::*)(const Vector2&) const>(&Vector2::operator-),
        sol::meta_function::multiplication, static_cast<Vector2 (Vector2::*)(float) const>(&Vector2::operator*),
        sol::meta_function::division, static_cast<Vector2 (Vector2::*)(float) const>(&Vector2::operator/)
    );

    lua.new_usertype<PositionProxy>("PositionProxy",
        "x", sol::property(&PositionProxy::getX, &PositionProxy::setX),
        "y", sol::property(&PositionProxy::getY, &PositionProxy::setY),

        "set", sol::overload(
            static_cast<void (PositionProxy::*)(float, float)>(&PositionProxy::set),
            static_cast<void (PositionProxy::*)(const Vector2&)>(&PositionProxy::set)
        ),
        "move", sol::overload(
            static_cast<void (PositionProxy::*)(float, float)>(&PositionProxy::move),
            static_cast<void (PositionProxy::*)(const Vector2&)>(&PositionProxy::move)
        ),

        sol::meta_function::addition,       &PositionProxy::add,
        sol::meta_function::subtraction,    &PositionProxy::sub,
        sol::meta_function::multiplication, &PositionProxy::mul,
        sol::meta_function::division,       &PositionProxy::div
    );

    lua.new_usertype<ScaleProxy>("ScaleProxy",
        "x", sol::property(&ScaleProxy::getX, &ScaleProxy::setX),
        "y", sol::property(&ScaleProxy::getY, &ScaleProxy::setY),

        "set", sol::overload(
            static_cast<void (ScaleProxy::*)(float, float)>(&ScaleProxy::set),
            static_cast<void (ScaleProxy::*)(const Vector2&)>(&ScaleProxy::set)
        ),
        "move", sol::overload(
            static_cast<void (ScaleProxy::*)(float, float)>(&ScaleProxy::move),
            static_cast<void (ScaleProxy::*)(const Vector2&)>(&ScaleProxy::move)
        ),

        sol::meta_function::addition,       &ScaleProxy::add,
        sol::meta_function::subtraction,    &ScaleProxy::sub,
        sol::meta_function::multiplication, &ScaleProxy::mul,
        sol::meta_function::division,       &ScaleProxy::div
    );

    lua.new_usertype<SDL_Color>("Color",
        sol::factories(
            []() {
                return SDL_Color{0, 0, 0, 255};
            },
            [](Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
                return SDL_Color{r, g, b, a};
            },
            [](Uint8 r, Uint8 g, Uint8 b) {
                return SDL_Color{r, g, b, 255};
            }
            ),
        "r", &SDL_Color::r,
        "g", &SDL_Color::g,
        "b", &SDL_Color::b,
        "a", &SDL_Color::a
    );
}