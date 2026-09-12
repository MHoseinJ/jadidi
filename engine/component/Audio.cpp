#include "Audio.h"
#include "core/Audio.h"

void Audio::Play(const std::string& audio_name, const int loop) {
    name = audio_name;
    loops = loop;
    if (const auto channel_played_on = AudioSystem::getInstance().PlaySound(audio_name, -1, loops);
        channel_played_on < 0) {
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

void Audio::DeSerialize(const Json& j) {
    name = j.get<std::string>("name", "");
    loops = j.get<int>("loops", 1);
    volume = j.get<int>("volume", 128);
    spatial = j.get<bool>("spatial", false);
    maxDistance = j.get<float>("maxDistance", 5000.0f);
    channel = j.get<int>("channel", -1);
}

float CalculateSpatialVolume(const float distance, const float maxDistance, const float volume) {
    if (distance > maxDistance) {
        return 0.0f;
    }
    const float normalized_distance = distance / maxDistance;
    return (1.0f - normalized_distance) * volume;
}