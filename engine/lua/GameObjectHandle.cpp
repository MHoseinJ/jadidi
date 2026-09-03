#include "GameObjectHandle.h"

#include <string>

#include "core/Log.h"
#include "scene/GameObject.h"
#include "scene/SceneManager.h"

GameObjectHandle::GameObjectHandle(uint64_t id_) : id(id_) {}

bool GameObjectHandle::isValid() const {
    if (id == 0)
        return false;

    return SceneManager::getInstance().findGameObjectWithId(id) != nullptr;
}

GameObject* GameObjectHandle::resolve() const {
    if (id == 0)
        return nullptr;

    return SceneManager::getInstance().findGameObjectWithId(id);
}

GameObject* GameObjectHandle::resolveOrLog() const {
    GameObject* go = resolve();

    if (!go) {
        if (!invalidLogged) {
            gameLog("[Lua] GameObjectHandle(id=" + std::to_string(id) + ") is invalid or has been destroyed.", ERROR);
            invalidLogged = true;
        }

        return nullptr;
    }

    return go;
}

bool GameObjectHandle::operator==(const GameObjectHandle& other) const {
    return id == other.id;
}