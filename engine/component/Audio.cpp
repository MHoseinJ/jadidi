#include "Audio.h"
#include "core/Audio.h"

void Audio::Play(const std::string &audio_name, const int loop) {
    name = audio_name;
    loops = loop;

    if (const auto channel_played_on = AudioSystem::getInstance().PlaySound(audio_name, -1, loops); channel_played_on < 0) {
        gameLog("failed to play sound: " + audio_name, ERROR);
        channel = channel_played_on;
    }
}

void Audio::Stop() {
    AudioSystem::getInstance().StopSpecificSoundEffect(name);
    name = "";
}

void Audio::Update(float dt) {

    if (spatial) {
        const float distance = owner->transform.position.distance(camera.transform.position);
        auto final_volume = CalculateSpatialVolume(distance, maxDistance, static_cast<float>(volume));

        AudioSystem::getInstance().SetSpecificSoundVolume(name, static_cast<int>(final_volume));
    }

}

int Audio::GetVolume() const {
    return volume;
}

void Audio::SetVolume(const int volume_in) {
    volume = volume_in;
    AudioSystem::getInstance().SetSpecificSoundVolume(name, static_cast<int>(volume_in));
}

void Audio::DeSerialize(const json &j) {
    if (j.contains("name")) {
        name = j["name"].get<std::string>();
    }
    if (j.contains("loops")) {
        loops = j["loops"].get<int>();
    }
    if (j.contains("volume")) {
        volume = j["volume"].get<int>();
    }
    if (j.contains("spatial")) {
        spatial = j["spatial"].get<bool>();
    }
    if (j.contains("maxDistance")) {
        maxDistance = j["maxDistance"].get<float>();
    }
    if (j.contains("channel")) {
        channel = j["channel"].get<int>();
    }
}

float CalculateSpatialVolume(const float distance, const float maxDistance, const float volume = 128) {
    if (distance > maxDistance) {
        return 0.0f;
    }

    const float normalized_distance = distance / maxDistance;
    return (1.0f - normalized_distance) * volume;
}