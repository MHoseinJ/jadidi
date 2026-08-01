#include "Scene.h"

GameObject* Scene::createObject(const std::string& name)
{
    auto obj = std::make_unique<GameObject>();

    obj->id = objects.size() + 1;
    obj->name = name;

    GameObject* ptr = obj.get();

    objects.push_back(std::move(obj));

    return ptr;
}

void Scene::onEnter() {
    for (auto& obj : objects) {
        for (auto& [_, comp] : obj->components)
            comp->OnCreate();
    }
}

void Scene::onExit() {
    for (auto& obj : objects) {
        for (auto& [_, comp] : obj->components)
            comp->OnDestroy();
    }
}
