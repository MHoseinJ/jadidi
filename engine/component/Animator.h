#pragma once
#include "scene/GameObject.h"
#include <string>
#include <unordered_map>
#include <iostream>
#include "Animation.h"
#include "Component.h"
#include "Sprite.h"
#include "utils/FileSystem.h"

struct Animator final : Component {
    std::unordered_map<std::string, Animation> animations;

    std::string current_anim_name;
    Sprite* sprite = nullptr;

    int current_frame = 0;
    float accumulator = 0.0f;

    float speed = 1.0f;
    bool playing = true;

    std::string startAnim;
    bool initialized = false;

    void OnCreate() override;

    void Update(float dt) override;

    void Register(const Animation& anim);

    void Play(const std::string& name, bool restart = false);


    void Pause()  { playing = false; }
    void Resume() { playing = true; }

    void Stop();

    void SetSpeed(float s);

    void DeSerialize(const json& j) override;

private:
    const Animation* GetCurrentAnimation() const;

    void Init();

    void ApplyFrame() const;
};