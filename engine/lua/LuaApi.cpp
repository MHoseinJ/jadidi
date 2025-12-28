#include "LuaApi.h"

#include "LuaBindings.h"
#include "core/Log.h"

#include "core/Input.h"
#include "scene/SceneManager.h"

#include "core/Input.h"

void LuaApi::print(const std::string& str) {
    gameLog(str, LogType::PRINT);
}

void LuaApi::debug(const std::string& str) {
    gameLog(str, LogType::DEBUG);
}

void LuaApi::info(const std::string& str) {
    gameLog(str, LogType::INFO);
}

void LuaApi::warn(const std::string& str) {
    gameLog(str, LogType::WARNING);
}

void LuaApi::error(const std::string& str) {
    gameLog(str, LogType::ERROR);
}

std::string LuaApi::getObject(const std::string &name) {
    for (auto& check : currentScene) {
        if (check.name == name)
            return name;
    }
    gameLog("game object not found:" + name, LogType::ERROR);
    return "";
}

void LuaApi::moveObjectPosition(const std::string &name, Vector2 vector) {
    GameObject* object = SceneManager::findGameObjectWithName(name);
    if (object == nullptr) {
        gameLog("object not found:" + name, LogType::ERROR);
        return;
    }

    object->transform.position.x += vector.x;
    object->transform.position.y += vector.y;
}

void LuaApi::setObjectPosition(const std::string &name, Vector2 vector) {
    GameObject* object = SceneManager::findGameObjectWithName(name);
    if (object == nullptr) {
        gameLog("object not found:" + name, LogType::ERROR);
        return;
    }

    object->transform.position.x = vector.x;
    object->transform.position.y = vector.y;
}

void LuaApi::setObjectScale(const std::string &name, Vector2 vector) {
    GameObject* object = SceneManager::findGameObjectWithName(name);
    if (object == nullptr) {
        gameLog("object not found:" + name, LogType::ERROR);
        return;
    }

    object->transform.scale.x = vector.x;
    object->transform.scale.y = vector.y;

}

void LuaApi::switchScene(const std::string &name) {
    SceneManager::loadSceneJson(name);
    Lua::loadSceneScripts(name);
}

void LuaApi::exit(int code) {
    Input::Quit();

}

Vector2 LuaApi::getObjectPosition(const std::string &name) {
    const GameObject* object = SceneManager::findGameObjectWithName(name);
    if (object == nullptr) {
        gameLog("object not found:" + name, LogType::ERROR);
        return Vector2{0,0};
    }
    return object->transform.position;
}

void LuaApi::clear() {
    clearAllLogs();
}

bool LuaApi::isKeyPressed(const std::string& key) {
    return Input::IsKeyPressed(key);
}

bool LuaApi::isKeyDown(const std::string& key) {
    return Input::IsKeyDown(key);
}

bool LuaApi::isKeyUp(const std::string& key) {
    return Input::IsKeyUp(key);
}

bool LuaApi::isMousePressed(const std::string& button) {
    return Input::IsMouseButtonPressed(button);
}

bool LuaApi::isMouseDown(const std::string& button) {
    return Input::IsMouseButtonDown(button);
}

bool LuaApi::isMouseUp(const std::string& button) {
    return Input::IsMouseButtonUp(button);
}

Vector2 LuaApi::getMousePosition() {
    int x, y;
    Input::GetMousePosition(x, y);
    return Vector2{(float)x, (float)y};
}
