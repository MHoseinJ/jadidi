#pragma once
#include <string>
#include <sol/state.hpp>

#include "core/Log.h"
#include "scene/GameObject.h"
#include "scene/SceneManager.h"
#include "utils/math/vector.h"

class LuaApi {
public:

    // logging
    static void print(const std::string& str);
    static void debug(const std::string& str);
    static void error(const std::string& str);
    static void warn(const std::string& str);
    static void info(const std::string& str);
    static void clear();

    // game objects
    static GameObject& getObject(const std::string& name);
    static void moveObjectPosition(const std::string& name, Vector2 vector);
    static void setObjectPosition(const std::string& name, Vector2 vector);
    static void setObjectScale(const std::string& name, Vector2 vector);
    static Vector2 getObjectPosition(const std::string& name);

    // vector
    // static void move(Vector2& diff);

    // engine
    static void switchScene(const std::string& name);
    static void exit();

    // number
    static bool isKeyPressed(int scancode);
    static bool isKeyDown(int scancode);
    static bool isKeyUp(int scancode);

    static bool isMousePressed(int button);
    static bool isMouseDown(int button);
    static bool isMouseUp(int button);

    // string overload
    static bool isKeyPressed(const std::string& key);
    static bool isKeyDown(const std::string& key);
    static bool isKeyUp(const std::string& key);

    static bool isMousePressed(const std::string& button);
    static bool isMouseDown(const std::string& button);
    static bool isMouseUp(const std::string& button);

    static Vector2 getMousePosition();

    // bindings
    static void bindKeys(sol::state& lua);
    static void bindMouse(sol::state& lua);

    static void debugger(Vector2& v2);
};