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

    // engine
    static void switchScene(const std::string& name);
    static Vector2 getScreenSize();
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

    static Vector2 getMouseWorldPosition();

    // bindings
    static void bindKeys(sol::state& lua);
    static void bindMouse(sol::state& lua);

    // component management
    static Component* addComponent(GameObject &go, const std::string &name);
    static Component* getComponent(GameObject &go, const std::string &name);

    static sol::object LuaJSON(nlohmann::json &json);
    static nlohmann::json LuaJSON(const sol::object& obj);

    static sol::function getFunctionByName(const std::string &name);
};