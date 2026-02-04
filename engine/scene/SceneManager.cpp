#include "SceneManager.h"

#include "core/Engine.h"
#include "core/Log.h"
#include "render/TextureManager.h"
#include "utils/FileSystem.h"
#include "utils/json.hpp"
#include "GameObject.h"
#include "render/Renderer.h"

std::vector<GameObject> currentScene;

static GameObject dummyObject{};

void SceneManager::loadSceneJson(const std::string& sceneName) {

    for (auto& obj : currentScene) {
        if (obj.sprite.texture) {
            SDL_DestroyTexture(obj.sprite.texture);
            obj.sprite.texture = nullptr;
        }
    }
    currentScene.clear();

    json data = fs::readJson("Scenes/" + sceneName + ".json");

    for (const auto& item : data["objects"]) {
        GameObject object;

        object.id = nextID;

        object.name = item["name"];

        object.transform.position.x = item["transform"]["position"]["x"];
        object.transform.position.y = item["transform"]["position"]["y"];

        object.transform.scale.x = item["transform"]["scale"]["x"];
        object.transform.scale.y = item["transform"]["scale"]["y"];

        auto texPath = item["sprite"]["texture"].get<std::string>();
        object.sprite.texture = createImageTexture(texPath, renderer);

        currentScene.push_back(object);

        nextID++;
    }

}

std::vector<GameObject> &SceneManager::getCurrentScene() {
    return currentScene;
}

GameObject& SceneManager::createObject(const std::string &name) {
    dirtyList = true;

    currentScene.emplace_back();
    GameObject& object = currentScene.back();

    object.name = name;
    object.tag = "";
    object.id = nextID++;
    object.transform.position = {0, 0};
    object.transform.scale = {1, 1};
    object.sprite.texture = nullptr;

    return object;
}


void SceneManager::deleteObjectByNameAndTag(const std::string &name, const std::string &tag) {
    for (auto it = currentScene.begin(); it != currentScene.end(); ) {
        if (it->name == name && it->tag == tag) {
            if (it->sprite.texture) {
                SDL_DestroyTexture(it->sprite.texture);
                it->sprite.texture = nullptr;
            }
            it = currentScene.erase(it);
        } else {
            ++it;
        }
    }
}

void SceneManager::deleteObjectById(int id) {
    for (auto it = currentScene.begin(); it != currentScene.end(); ++it) {
        if (it->id == static_cast<uint64_t>(id)) {
            if (it->sprite.texture) {
                SDL_DestroyTexture(it->sprite.texture);
                it->sprite.texture = nullptr;
            }
            currentScene.erase(it);
            return;
        }
    }

    gameLog("no game object found with id: " + std::to_string(id), WARNING);
}

void SceneManager::deleteAllObjectsByTag(const std::string &tag) {
    for (auto it = currentScene.begin(); it != currentScene.end(); ) {
        if (it->tag == tag) {
            if (it->sprite.texture) {
                SDL_DestroyTexture(it->sprite.texture);
                it->sprite.texture = nullptr;
            }
            it = currentScene.erase(it);
        } else {
            ++it;
        }
    }
}

void SceneManager::deleteAllObjectsByName(const std::string &name) {
    for (auto it = currentScene.begin(); it != currentScene.end(); ) {
        if (it->name == name) {
            if (it->sprite.texture) {
                SDL_DestroyTexture(it->sprite.texture);
                it->sprite.texture = nullptr;
            }
            it = currentScene.erase(it);
        } else {
            ++it;
        }
    }
}


GameObject &SceneManager::findGameObjectWithName(const std::string& name) {
    for (auto& object : currentScene) {
        if (object.name == name) {
            return object;
        }
    }

    gameLog("no game object found with name: " + name, ERROR);
    return dummyObject;
}

GameObject &SceneManager::findGameObjectWithId(const int id) {
    for (auto& object : currentScene) {
        if (object.id == static_cast<uint64_t>(id)) {
            return object;
        }
    }

    gameLog("no game object found with id: " + std::to_string(id), ERROR);
    return dummyObject;
}