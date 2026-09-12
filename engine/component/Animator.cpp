#include "Animator.h"
#include "core/Log.h"

void Animator::OnCreate() {
    sprite = owner->getComponent<Sprite>();
}

void Animator::Update(const float dt) {
    if (!initialized) {
        Init();
        initialized = true;
    }

    if (!sprite) {
        sprite = owner->getComponent<Sprite>();
    }

    const Animation* anim = GetCurrentAnimation();
    if (!playing || !anim || !sprite)
        return;

    accumulator += dt * speed;
    while (accumulator >= anim->frameTime) {
        accumulator -= anim->frameTime;
        current_frame++;
        if (current_frame > anim->endFrame) {
            if (anim->loop) {
                current_frame = anim->startFrame;
            } else {
                playing = false;
                current_anim_name.clear();
                return;
            }
        }
    }
    ApplyFrame();
}

void Animator::Register(const Animation& anim) {
    animations[anim.name] = anim;
}

void Animator::Play(const std::string& name, bool restart) {
    if (name == current_anim_name && !restart)
        return;

    auto it = animations.find(name);
    if (it == animations.end()) {
        Json json = fs::readJsonNew("Animations/" + name + ".json");
        if (!json.isValid() || !json.raw().is_object()) {
            gameLog("Invalid animation json: " + name, ERROR);
            return;
        }
        Animation anim;
        anim.name = name;
        anim.DeSerialize(json);
        auto [newIt, ok] = animations.emplace(name, std::move(anim));
        it = newIt;
    }

    current_anim_name = name;
    current_frame = it->second.startFrame;
    accumulator = 0.0f;
    playing = true;
    ApplyFrame();
}

void Animator::Stop() {
    playing = false;
    current_anim_name.clear();
    current_frame = 0;
}

void Animator::SetSpeed(float s) {
    speed = std::max(0.0f, s);
}

void Animator::DeSerialize(const Json& j) {
    speed = j.get<float>("speed", 1.0f);
    playing = j.get<bool>("playing", true);
    startAnim = j.get<std::string>("play", "");
}

const Animation* Animator::GetCurrentAnimation() const {
    if (current_anim_name.empty())
        return nullptr;
    auto it = animations.find(current_anim_name);
    return (it != animations.end()) ? &it->second : nullptr;
}

void Animator::Init() {
    if (!startAnim.empty()) {
        Play(startAnim, true);
    }
}

void Animator::ApplyFrame() const {
    const Animation* anim = GetCurrentAnimation();
    if (!sprite || !anim)
        return;
    sprite->srcRect.x = current_frame * anim->frameW;
    sprite->srcRect.y = 0;
    sprite->srcRect.w = anim->frameW;
    sprite->srcRect.h = anim->frameH;
}