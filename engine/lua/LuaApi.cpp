#include "LuaApi.h"

#include "LuaBindings.h"
#include "core/Log.h"
#include "scene/SceneManager.h"

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
    return nullptr;
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

Vector2 LuaApi::getObjectPosition(const std::string &name) {
    const GameObject* object = SceneManager::findGameObjectWithName(name);
    if (object == nullptr) {
        gameLog("object not found:" + name, LogType::ERROR);
    }
    return object->transform.position;
}
