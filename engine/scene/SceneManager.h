#pragma once
#include <vector>
#include "utils/json.hpp"
#include "GameObject.h"

using json = nlohmann::json;

extern std::vector<GameObject> currentScene;
inline uint64_t nextID = 1;

namespace SceneManager {
    void loadSceneJson(const std::string& sceneName);
    std::vector<GameObject>& getCurrentScene();
    GameObject& findGameObjectWithName(const std::string& name);
    GameObject& findGameObjectWithId(int id);
    GameObject& createObject(const std::string &name);
    void deleteObjectByNameAndTag(const std::string &name, const std::string &tag);
    void deleteObjectById(int id);
    void deleteAllObjectsByTag(const std::string &tag);
    void deleteAllObjectsByName(const std::string &name);
}