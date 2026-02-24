#include "SceneManager.h"
#include "utils/FileSystem.h"
#include "component/Factory.h"
#include "core/Log.h"

using json = nlohmann::json;

void SceneManager::loadScene(const std::string& sceneName) {
    // clear current scene (calls onExit on all GameObjects)
    currentScene.onExit();
    currentScene.objects.clear();

    // read JSON
    json data = fs::readJson("Scenes/" + sceneName + ".json");

    // load objects
    for (const auto& item : data["objects"]) {
        GameObject& obj = currentScene.createObject(item["name"]);

        if (item.contains("tag"))
            obj.tag = item["tag"];

        // add components using Factory
        for (auto& [key, value] : item.items()) {
            if (key == "name" || key == "tag" || key == "transform" || key == "id")
                continue;

            auto comp = Factory::instance().create(key);
            comp->DeSerialize(value);
            obj.addComponent(std::move(comp));
        }

        // transform deserialization (Transform is mandatory)
        if (item.contains("transform")) {
            obj.transform.DeSerialize(item["transform"]);
        }
    }

    currentScene.onEnter();
}

Scene& SceneManager::getCurrentScene() {
    return currentScene;
}

GameObject& SceneManager::createObject(const std::string& name) {
    return currentScene.createObject(name);
}

void SceneManager::deleteObjectById(uint64_t id) {
    auto& objs = currentScene.objects;
    objs.erase(std::remove_if(objs.begin(), objs.end(),
        [id](GameObject& obj) { return obj.id == id; }),
        objs.end());
}

void SceneManager::deleteObjectByNameAndTag(const std::string& name, const std::string& tag) {
    auto& objs = currentScene.objects;
    objs.erase(std::remove_if(objs.begin(), objs.end(),
        [&](GameObject& obj) { return obj.name == name && obj.tag == tag; }),
        objs.end());
}

void SceneManager::deleteAllObjectsByName(const std::string& name) {
    auto& objs = currentScene.objects;
    objs.erase(std::remove_if(objs.begin(), objs.end(),
        [&](GameObject& obj) { return obj.name == name; }),
        objs.end());
}

void SceneManager::deleteAllObjectsByTag(const std::string& tag) {
    auto& objs = currentScene.objects;
    objs.erase(std::remove_if(objs.begin(), objs.end(),
        [&](GameObject& obj) { return obj.tag == tag; }),
        objs.end());
}

GameObject* SceneManager::findGameObjectWithName(const std::string& name) {
    for (auto& obj : currentScene.objects) {
        if (obj.name == name) return &obj;
    }
    return nullptr;
}

GameObject* SceneManager::findGameObjectWithId(uint64_t id) {
    for (auto& obj : currentScene.objects) {
        if (obj.id == id) return &obj;
    }
    return nullptr;
}