#pragma once
#include "Component.h"
#include "Sprite.h"

struct Animation {
    std::string name;

    int frameW = 0;
    int frameH = 0;

    int startFrame = 0;
    int endFrame = 0;

    float frameTime = 0.1f;
    bool loop = true;

    void DeSerialize(const json& j) {
        name       = j.value("name", "");
        frameW     = j.value("frame_w", 0);
        frameH     = j.value("frame_h", 0);
        startFrame = j.value("start", 0);
        endFrame   = j.value("end", startFrame);
        frameTime  = j.value("frame_time", 0.1f);
        loop       = j.value("loop", false);
    }
};

