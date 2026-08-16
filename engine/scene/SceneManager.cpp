#include "SceneManager.h"
#include "core/Input.h"
#include "utils/FileSystem.h"
#include "component/Factory.h"
#include "core/Log.h"
#include <string>

using json = nlohmann::json;

void SceneManager::loadScene(const std::string& sceneName) {
    // clear current scene (calls onExit on all GameObjects)
    currentScene.onExit();
    currentScene.objects.clear();
    idLookup.clear();

    // read JSON
    json data = fs::readJson("Scenes/" + sceneName + ".json");

    // load objects
    for (const auto& item : data["objects"]) {
        GameObject* obj = currentScene.createObject(item["name"]);
        idLookup[obj->id] = obj;

        if (item.contains("tag"))
            obj->tag = item["tag"];

        // add components using Factory
        for (auto& [key, value] : item.items()) {
            if (key == "name" || key == "tag" || key == "transform" || key == "id")
                continue;

            auto comp = Factory::instance().create(key);

            // check if available
            if (!comp) {
                gameLog("Unknown Component type while loading \"" + std::string(item["name"]) + "\": " + key, ERROR);
                continue;
            }
            
            comp->DeSerialize(value);
            obj->addComponent(std::move(comp));
        }

        // transform deserialization (Transform is mandatory)
        if (item.contains("transform")) {
            obj->transform.DeSerialize(item["transform"]);
        }
    }

    currentScene.onEnter();
}

Scene& SceneManager::getCurrentScene() {
    return currentScene;
}

GameObject& SceneManager::createObject(const std::string& name) {
    GameObject* obj = currentScene.createObject(name);
    idLookup[obj->id] = obj;
    return *obj;
}

void SceneManager::destroyGameObject(GameObject& obj) {
    for (auto& [type, component] : obj.components) {
        component->OnDestroy();
    }
}

void SceneManager::deleteObjectById(uint64_t id)
{
    idLookup.erase(id);
    
    auto& objs = currentScene.objects;

    auto it = std::find_if(
        objs.begin(),
        objs.end(),
        [id](const auto& obj) {
            return obj->id == id;
        }
    );

    if (it == objs.end())
        return;

    destroyGameObject(**it);
    objs.erase(it);
}

void SceneManager::deleteObjectByNameAndTag(
    const std::string& name,
    const std::string& tag
) {
    auto& objs = currentScene.objects;

    objs.erase(
        std::remove_if(objs.begin(), objs.end(),
            [&](const std::unique_ptr<GameObject>& obj) {
                if (obj->name == name && obj->tag == tag) {
                    idLookup.erase(obj->id);
                    destroyGameObject(*obj);
                    return true;
                }
                return false;
            }),
        objs.end()
    );
}

void SceneManager::deleteAllObjectsByName(const std::string& name) {
    auto& objs = currentScene.objects;

    objs.erase(
        std::remove_if(objs.begin(), objs.end(),
            [&](const std::unique_ptr<GameObject>& obj) {
                if (obj->name == name) {
                    idLookup.erase(obj->id);
                    destroyGameObject(*obj);
                    return true;
                }
                return false;
            }),
        objs.end()
    );
}

void SceneManager::deleteAllObjectsByTag(const std::string& tag) {
    auto& objs = currentScene.objects;

    objs.erase(
        std::remove_if(objs.begin(), objs.end(),
            [&](const std::unique_ptr<GameObject>& obj) {
                if (obj->tag == tag) {
                    idLookup.erase(obj->id);
                    destroyGameObject(*obj);
                    return true;
                }
                return false;
            }),
        objs.end()
    );
}

GameObject* SceneManager::findGameObjectWithName(const std::string& name) {
    for (auto& obj : currentScene.objects) {
        if (obj->name == name) return obj.get();
    }
    return nullptr;
}

GameObject* SceneManager::findGameObjectWithId(uint64_t id) {
    auto it = idLookup.find(id);
    if (it != idLookup.end()) {
        return it->second;
    }
    return nullptr;
}