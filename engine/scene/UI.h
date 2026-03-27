#pragma once
#include "GameObject.h"
#include "component/Collider.h"
#include "utils/math/vector.h"

class UIManager {
    public:
    static UIManager* getInstance();

    void Update();
};
