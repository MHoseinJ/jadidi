#include "Scene.h"

GameObject &Scene::createObject(const std::string &name) {
    objects.emplace_back();
    auto& obj = objects.back();
    obj.id = objects.size();
    obj.name = name;
    return obj;
}

void Scene::onEnter() {
    for (auto& obj : objects) {
        for (auto& [_, comp] : obj.components)
            comp->OnCreate();
    }
}

void Scene::onExit() {
    for (auto& obj : objects) {
        for (auto& [_, comp] : obj.components)
            comp->OnDestroy();
    }
}
