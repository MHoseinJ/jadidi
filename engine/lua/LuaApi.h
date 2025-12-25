#pragma once
#include <string>

#include "component/Transform.h"

class LuaApi {
    public:

    static void print(const std::string& str);
    static void debug(const std::string& str);
    static void error(const std::string& str);
    static void warn(const std::string& str);
    static void info(const std::string& str);
    static void clear();

    static std::string getObject(const std::string& name);
    static void moveObjectPosition(const std::string& name, Vector2 vector);
    static void setObjectPosition(const std::string& name, Vector2 vector);
    static void setObjectScale(const std::string& name, Vector2 vector);
    static Vector2 getObjectPosition(const std::string& name);

    static void switchScene(const std::string& name);

    static bool isKeyPressed(const std::string& key);
    static bool isKeyDown(const std::string& key);
    static bool isKeyUp(const std::string& key);

    static bool isMousePressed(const std::string& button);
    static bool isMouseDown(const std::string& button);
    static bool isMouseUp(const std::string& button);

    static Vector2 getMousePosition();

};