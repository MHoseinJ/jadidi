#include "UI.h"

#include "SceneManager.h"
#include "component/Button.h"
#include "component/Colider.h"
#include "core/Input.h"
#include "lua/LuaApi.h"

UIManager *UIManager::getInstance() {
    static UIManager instance;
    return &instance;
}

void UIManager::Update() {
    const auto mousePos = Input::GetMouseWorldPos();
    for (int i = 1; i <= 3;) {
        if (Input::IsMouseButtonPressed(i)) {
            auto &[objects, camera] = SceneManager::getInstance().getCurrentScene();
            for (auto &object : objects) {
                std::cout << object.name << std::endl;
                const auto collision = object.getComponent<BoxCollider>();
                if (!collision) continue;
                const auto button = object.getComponent<Button>();
                if (!button) continue;

                std::cout << IsColliding(mousePos, *collision) << std::endl;
                if (IsColliding(mousePos, *collision)) {
                    std::cout << object.name << " function called" << std::endl;
                    button->callFunction(i);
                }
            }
        }
        i++;
    }
}
