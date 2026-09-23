#include "SceneManager.h"
#include "core/Engine.h"
#include "core/Input.h"
#include "scene/Scene.h"
#include "utils/FileSystem.h"
#include "component/Factory.h"
#include "core/Log.h"
#include <string>
#include "component/LuaComponent.h"

void SceneManager::loadScene(const std::string& sceneName)
{
    currentScene.onExit();
    currentScene.objects.clear();
    idLookup.clear();

    if (physics.has_value())
        physics.reset();

    Json data = fs::readJsonNew("Scenes/" + sceneName + ".json");

    if (!data.isValid()) {
        gameLog("Failed to load scene: " + sceneName, ERROR);
        return;
    }

    try {

        Vector2 gravity = {0.0f, -9.8f};

        if (data.has("physics")) {
            Json physicsJson = data.getObject("physics");

            if (physicsJson.has("gravity")) {
                gravity = physicsJson.get<Vector2>(
                    "gravity",
                    {0.0f, -9.8f}
                );
            }
        }

        physics.emplace(gravity);

        gameLog(
            "[Scene] Physics gravity set to (" +
            std::to_string(gravity.x) + ", " +
            std::to_string(gravity.y) + ")",
            INFO
        );

        if (!data.has("objects")) {
            gameLog(
                "Scene file missing 'objects' array",
                ERROR
            );
            return;
        }

        Json objectsArray = data.getArray("objects");

        gameLog(
            "[Scene] Found " +
            std::to_string(objectsArray.size()) +
            " root object(s) in scene '" +
            sceneName +
            "'",
            INFO
        );

        std::function<GameObject*(const Json&, GameObject*)> loadObject;

        loadObject = [&](const Json& item, GameObject* parent) -> GameObject*
        {
            if (!item.has("name")) {
                gameLog(
                    "[Scene] Object missing 'name' field — skipped",
                    ERROR
                );
                return nullptr;
            }

            std::string objName =
                item.get<std::string>("name");

            GameObject* obj =
                currentScene.createObject(objName);

            if (item.has("id")) {
                obj->id = static_cast<uint64_t>(
                    item.get<int>(
                        "id",
                        static_cast<int>(obj->id)
                    )
                );
            }

            idLookup[obj->id] = obj;

            if (obj->id >= currentScene.nextId) {
                currentScene.nextId =
                    static_cast<uint32_t>(obj->id + 1);
            }

            if (item.has("tag")) {
                obj->tag =
                    item.get<std::string>("tag");
            }

            gameLog(
                "[Scene] Loading object '" +
                objName +
                "' (id=" +
                std::to_string(obj->id) +
                ", tag='" +
                obj->tag +
                "')",
                INFO
            );

            if (item.has("transform")) {
                Json transformJson =
                    item.getObject("transform");

                obj->transform.DeSerialize(
                    transformJson
                );
            }

            if (parent) {
                if (!obj->setParent(parent, false)) {
                    gameLog(
                        "[Scene] Failed to parent '" +
                        objName +
                        "' to '" +
                        parent->name +
                        "'",
                        ERROR
                    );
                }
            }

            for (const auto& [key, value] : item.raw().items()) {

                if (
                    key == "name" ||
                    key == "tag" ||
                    key == "transform" ||
                    key == "id" ||
                    key == "children"
                ) {
                    continue;
                }

                if (key.rfind("lua:", 0) != 0)
                    continue;

                auto comp =
                    Factory::instance().create(key);

                if (!comp) {
                    gameLog(
                        "Unknown component '" +
                        key +
                        "' — skipped",
                        ERROR
                    );
                    continue;
                }

                Json compJson(
                    &value,
                    objName + " -> " + key
                );

                comp->owner = obj;

                if (auto* luaComp =
                        dynamic_cast<LuaComponent*>(comp.get())) {
                    luaComp->awake();
                }

                comp->DeSerialize(compJson);

                obj->addComponent(
                    std::move(comp)
                );
            }

            for (const auto& [key, value] : item.raw().items()) {

                if (
                    key == "name" ||
                    key == "tag" ||
                    key == "transform" ||
                    key == "id" ||
                    key == "children"
                ) {
                    continue;
                }

                if (key.rfind("lua:", 0) == 0)
                    continue;

                auto comp =
                    Factory::instance().create(key);

                if (!comp) {
                    gameLog(
                        "Unknown component '" +
                        key +
                        "' — skipped",
                        ERROR
                    );
                    continue;
                }

                Json compJson(
                    &value,
                    objName + " -> " + key
                );

                comp->DeSerialize(compJson);

                obj->addComponent(
                    std::move(comp)
                );
            }

            if (item.has("children")) {

                Json children =
                    item.getArray("children");

                for (size_t i = 0;
                     i < children.size();
                     ++i) {

                    loadObject(
                        children[i],
                        obj
                    );
                }
            }

            return obj;
        };

        for (size_t i = 0;
             i < objectsArray.size();
             ++i) {

            loadObject(
                objectsArray[i],
                nullptr
            );
        }
    }
    catch (const JsonException& e) {

        gameLog(
            e.what(),
            ERROR
        );

        gameLog(
            "Failed to load scene '" +
            sceneName +
            "'. Check your JSON files for errors.",
            ERROR
        );
    }

    gameLog(
        "[Scene] Scene '" +
        sceneName +
        "' fully loaded. Total objects: " +
        std::to_string(currentScene.objects.size()),
        INFO
    );

    currentScene.onEnter();
}

void SceneManager::requestSwitchScene(const std::string& sceneName) {
    pendingScene = sceneName;
}

void SceneManager::processPendingScene() {
    if (pendingScene.empty()) { return; }
    
    loadScene(pendingScene);
    pendingScene = "";
}

Scene& SceneManager::getCurrentScene() {
    return currentScene;
}

GameObject& SceneManager::createObject(const std::string& name) {
    GameObject* obj = currentScene.createObject(name);
    idLookup[obj->id] = obj;
    return *obj;
}

void SceneManager::destroyGameObject(GameObject& obj)
{
    // detatch from hirarch before deleting it
    obj.detachFromHierarchy();

    for (auto& [type, component] : obj.components) {
        component->OnDestroy();
    }
}

void SceneManager::deleteObjectById(uint64_t id)
{
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

    idLookup.erase(id);

    objs.erase(it);
}

void SceneManager::deleteObjectByNameAndTag(
    const std::string& name,
    const std::string& tag)
{
    auto& objs = currentScene.objects;

    objs.erase(
        std::remove_if(
            objs.begin(),
            objs.end(),
            [&](const std::unique_ptr<GameObject>& obj) {
                if (obj->name == name && obj->tag == tag) {
                    idLookup.erase(obj->id);
                    destroyGameObject(*obj);
                    return true;
                }

                return false;
            }
        ),
        objs.end()
    );
}

void SceneManager::deleteAllObjectsByName(const std::string& name)
{
    auto& objs = currentScene.objects;

    objs.erase(
        std::remove_if(
            objs.begin(),
            objs.end(),
            [&](const std::unique_ptr<GameObject>& obj) {
                if (obj->name == name) {
                    idLookup.erase(obj->id);
                    destroyGameObject(*obj);
                    return true;
                }

                return false;
            }
        ),
        objs.end()
    );
}

void SceneManager::deleteAllObjectsByTag(const std::string& tag)
{
    auto& objs = currentScene.objects;

    objs.erase(
        std::remove_if(
            objs.begin(),
            objs.end(),
            [&](const std::unique_ptr<GameObject>& obj) {
                if (obj->tag == tag) {
                    idLookup.erase(obj->id);
                    destroyGameObject(*obj);
                    return true;
                }

                return false;
            }
        ),
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
    return (it != idLookup.end()) ? it->second : nullptr;
}