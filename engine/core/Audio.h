#pragma once
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL.h>
#include <string>
#include <unordered_map>

enum SOUND_TYPE {
    CHUNK = 0,
    MUSIC = 1,
};

enum STATE_OF_PLAYING {
    PLAYING = 0,
    STOPPED = 1,
    PAUSED = 2,
};

struct Sound {
    SOUND_TYPE type;
    STATE_OF_PLAYING state;

    union {
        Mix_Chunk* chunk;
        Mix_Music* music;
    };

    int channel = -1;
};

class AudioSystem {
    public:
    AudioSystem();
    static AudioSystem& getInstance();
    ~AudioSystem();

    bool LoadSound(const std::string& name, const std::string &path, bool isMusic);
    int PlaySound(const std::string &name, int channel, int loops = 0);
    void StopSound();
    void StopSpecificSoundEffect(const std::string &name);

    static void SetMusicVolume(int volume);
    void SetSpecificSoundVolume(const std::string& name, int volume);
    void UnloadSound(const std::string &name);
    static bool IsChannelPlaying(int channel);
    void StopAllSoundEffects();

    AudioSystem(const AudioSystem&) = delete;
    AudioSystem& operator=(const AudioSystem&) = delete;
private:
    std::unordered_map<std::string, Sound> sounds;
};

// helper
std::string to_lower(std::string str);