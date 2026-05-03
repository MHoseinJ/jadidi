#include "Audio.h"
#include <algorithm>
#include <filesystem>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL.h>

#include "Log.h"

AudioSystem::AudioSystem() {

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        gameLog("sdl_mixer could not initialize: " + std::string(Mix_GetError()), ERROR);
        exit(-1);
    }

    Mix_AllocateChannels(8);

    gameLog("Audio initialized", INFO);
}

AudioSystem::~AudioSystem() {
    gameLog("Turning off audio system...", INFO);

    for (const auto&[fst, snd] : sounds) {
        if (snd.type == SOUND_TYPE::CHUNK) {
            Mix_FreeChunk(snd.chunk);
        } else if (snd.type == SOUND_TYPE::MUSIC) {
            Mix_FreeMusic(snd.music);
        }
    }

    sounds.clear();

    Mix_CloseAudio();
}

AudioSystem &AudioSystem::getInstance() {
    static AudioSystem instance;
    return instance;
}


bool AudioSystem::LoadSound(const std::string& name, const std::string &path, const bool isMusic = false) {

    gameLog("trying to load sound: '" + name + "'", INFO);
    if (sounds.count(name)) {
        gameLog("Sound already exists", INFO);
        return false;
    }

    Sound data;
    if (isMusic) {
        data.type = MUSIC;
        data.music = Mix_LoadMUS(path.c_str());
        if (!data.music) {
            gameLog("Failed to load music file: " + std::string(Mix_GetError()), ERROR);
            return false;
        }
    } else {
        data.type = CHUNK;
        data.chunk = Mix_LoadWAV(path.c_str());
        if (!data.chunk) {
            gameLog("Failed to load chunk file: " + std::string(Mix_GetError()), ERROR);
            return false;
        }
    }

    sounds[name] = data;
    gameLog("Loaded sound: " + name, INFO);

    return true;
}

int AudioSystem::PlaySound(const std::string &name, const int channel, const int loops) {

    gameLog("trying to play sound: " + name, INFO);
    for (const auto&[fst, snd] : sounds) {
        gameLog("list of sounds: " + name, INFO);
    }
    const auto it = sounds.find(name);
    if (it == sounds.end()) {
        gameLog("Sound not found: " + name, ERROR);
        return -1;
    }

    Sound& data = it->second;
    int actual_channel = -1;

    if (data.type == CHUNK) {
        if (channel != -1) {
            actual_channel = Mix_PlayChannel(channel, data.chunk, loops);
        } else {
            actual_channel = Mix_PlayChannel(-1, data.chunk, loops);

            if (actual_channel != -1) {
                data.channel = actual_channel;
            }
        }

        if (actual_channel == -1) {
            gameLog("Failed to play sound chunk '" + name + "'. Mix_PlayChannel error: " + std::string(Mix_GetError()), ERROR);
            return -1;
        }
    } else {
        if (Mix_PlayingMusic()) {
            Mix_HaltMusic();
        }
        if (Mix_PlayMusic(data.music, loops) == 0) {
            actual_channel = 0;
        } else {
            gameLog("Failed to play music '" + name + "': " + std::string(Mix_GetError()), ERROR);
            return -1;
        }
    }

    data.state = PLAYING;
    return actual_channel;
}


void AudioSystem::StopSound() {
    if (Mix_PlayingMusic()) {
        Mix_HaltMusic();
    }
    for (auto& sound : sounds) {
        if (sound.second.type == MUSIC) {
            sound.second.state = STOPPED;
        }
    }
}

void AudioSystem::StopAllSoundEffects() {
    Mix_HaltChannel(-1);
    for (auto& sound : sounds) {
        if (sound.second.type == CHUNK) {
            sound.second.state = STOPPED;
        }
    }
}

void AudioSystem::StopSpecificSoundEffect(const std::string& name) {
    if (!sounds.count(name)) return;
    Sound& data = sounds[name];
    if (data.type == CHUNK && data.channel != -1) {
        Mix_HaltChannel(data.channel);
        data.channel = -1;
        data.state = STOPPED;
    }
}

void AudioSystem::SetMusicVolume(const int volume) {
    Mix_VolumeMusic(volume);
}

void SetSoundEffectVolume(const int channel, const int volume) {
    Mix_Volume(channel, volume);
}

void AudioSystem::SetSpecificSoundVolume(const std::string& name, const int volume) {
    if (!sounds.count(name)) return;
    Sound& data = sounds[name];
    if (data.type == CHUNK && data.channel != -1) {
        Mix_Volume(data.channel, volume);
    } else if (data.type == MUSIC) {
        SetMusicVolume(volume);
    }
}

void AudioSystem::UnloadSound(const std::string &name) {
    if (!sounds.count(name)) return;
    Sound& data = sounds[name];
    if (data.type == CHUNK) {
        Mix_FreeChunk(data.chunk);
    } else {
        Mix_FreeMusic(data.music);
    }
    sounds.erase(name);
    gameLog("Unloaded sound: " + name, ERROR);
}

bool AudioSystem::IsChannelPlaying(const int channel) {
    return Mix_Playing(channel);
}

std::string to_lower(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), [](const unsigned char c){return std::tolower(c);});
    return str;
}
