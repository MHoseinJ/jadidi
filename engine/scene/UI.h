#pragma once
#include "GameObject.h"
#include "component/Colider.h"
#include "utils/math/vector.h"

class UIManager {
    public:
    static UIManager* getInstance();

    void Update();
};
