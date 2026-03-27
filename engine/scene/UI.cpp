#include "UI.h"

#include "SceneManager.h"
#include "component/Button.h"
#include "component/Collider.h"
#include "core/Input.h"
#include "lua/LuaApi.h"

UIManager *UIManager::getInstance() {
    static UIManager instance;
    return &instance;
}

void UIManager::Update() {
    const auto mousePos = Input::GetMouseWorldPos();
    int biggest_zorder = 0;
    Button* button_that_have_biggest_zOrder = nullptr;
    for (int i = 1; i <= 3;) {
        if (Input::IsMouseButtonPressed(i)) {
            auto &[objects, camera] = SceneManager::getInstance().getCurrentScene();
            for (auto &object : objects) {
                std::cout << object.name << std::endl;
                const auto collision = object.getComponent<BoxCollider>();

                if (!collision) continue;
                const auto button = object.getComponent<Button>();
                if (!button) continue;
                if (button->zOrder >= biggest_zorder && IsColliding(&mousePos, collision)) {
                    biggest_zorder = button->zOrder;
                    button_that_have_biggest_zOrder = button;
                }
            }
            if (button_that_have_biggest_zOrder != nullptr) {
                button_that_have_biggest_zOrder->callFunction(i);
            }
        }
        i++;
    }
}
