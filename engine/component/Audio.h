#pragma once
#include "Component.h"
#include "scene/GameObject.h"
#include "utils/math/vector.h"

struct Audio final : Component {
    std::string name;

    bool spatial = false;
    float maxDistance = 5000.0f;
    int volume = 128;
    int loops = 1;
    int channel = -1;

    void Play(const std::string& audio_name, int loop = 1);
    void Stop();
    void Update(float dt) override;

    void SetVolume(int volume_in);
    [[nodiscard]] int GetVolume() const;

    void DeSerialize(const json &j) override;
};

float CalculateSpatialVolume(float distance, float maxDistance, float volume);