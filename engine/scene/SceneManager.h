#pragma once
#include <vector>
#include "utils/json.hpp"
#include "GameObject.h"

using json = nlohmann::json;

extern std::vector<GameObject> currentScene;

namespace SceneManager {
    void loadSceneJson(const std::string& sceneName);
    std::vector<GameObject>& getCurrentScene();
    GameObject& findGameObjectWithName(const std::string& name);
    GameObject& findGameObjectWithId(int id);
}