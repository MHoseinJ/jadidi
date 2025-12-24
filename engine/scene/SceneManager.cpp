#include "SceneManager.h"

#include "core/Engine.h"
#include "core/Log.h"
#include "render/TextureManager.h"
#include "utils/FileSystem.h"
#include "utils/json.hpp"

std::vector<GameObject> currentScene;

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

        object.name = item["name"];

        object.transform.position.x = item["transform"]["position"]["x"];
        object.transform.position.y = item["transform"]["position"]["y"];

        object.transform.scale.x = item["transform"]["scale"]["x"];
        object.transform.scale.y = item["transform"]["scale"]["y"];

        auto texPath = item["sprite"]["texture"].get<std::string>();
        object.sprite.texture = createImageTexture(texPath, renderer);

        currentScene.push_back(object);
    }

}

std::vector<GameObject> &SceneManager::getCurrentScene() {
    return currentScene;
}

GameObject *SceneManager::findGameObjectWithName(const std::string& name) {
    for (auto& object : currentScene) {
        if (object.name == name) {
            return &object;
        }
    }
    gameLog("there is no game object found with that key: " + name, ERROR);
    return nullptr;
}
