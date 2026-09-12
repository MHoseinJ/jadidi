#pragma once
#include "utils/Json.h"
#include <string>

struct Animation {
    std::string name;
    int frameW = 0;
    int frameH = 0;
    int startFrame = 0;
    int endFrame = 0;
    float frameTime = 0.1f;
    bool loop = true;

    void DeSerialize(const Json& j) {
        name = j.get<std::string>("name", "");
        frameW = j.get<int>("frame_w", 0);
        frameH = j.get<int>("frame_h", 0);
        startFrame = j.get<int>("start", 0);
        endFrame = j.get<int>("end", startFrame);
        frameTime = j.get<float>("frame_time", 0.1f);
        loop = j.get<bool>("loop", false);
    }
};