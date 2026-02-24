#pragma once
#include <string>
#include "Scene.h"

class SceneManager {
public:

    static SceneManager& getInstance() {
        static SceneManager instance;
        return instance;
    }

    // load a scene from JSON file
    void loadScene(const std::string& sceneName);

    // access current scene
    Scene& getCurrentScene();

    // create / delete objects
    GameObject& createObject(const std::string& name);
    void deleteObjectById(uint64_t id);
    void deleteObjectByNameAndTag(const std::string& name, const std::string& tag);
    void deleteAllObjectsByName(const std::string& name);
    void deleteAllObjectsByTag(const std::string& tag);

    // find objects
    GameObject* findGameObjectWithName(const std::string& name);
    GameObject* findGameObjectWithId(uint64_t id);

private:
    Scene currentScene;
};
