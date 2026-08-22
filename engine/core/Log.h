#pragma once
#include <SDL2/SDL_render.h>
#include <deque>
#include <string>
#include "container/Queue.h"
#include "render/TextureHandle.h"

enum LogType {
    ERROR,
    WARNING,
    INFO,
    DEBUG,
    PRINT,
};

struct LogEntry {
    LogType type;
    std::string message;
    TextureHandle texture;

    LogEntry() : type(INFO), message(""), texture() {} 
    
    LogEntry(LogType t, std::string&& msg, TextureHandle tex = TextureHandle())
        : type(t), message(std::move(msg)), texture(tex) {}

    ~LogEntry() {
        if (texture.sdlTexture) {
            SDL_DestroyTexture(texture.sdlTexture);
            texture.sdlTexture = nullptr;
        }
    }

    LogEntry(const LogEntry&) = delete;
    LogEntry& operator=(const LogEntry&) = delete;

    LogEntry(LogEntry&& other) noexcept
        : type(other.type),
          message(std::move(other.message)),
          texture(other.texture)
    {
        other.texture.sdlTexture = nullptr; 
    }

    LogEntry& operator=(LogEntry&& other) noexcept {
        if (this != &other) {
            if (texture.sdlTexture) {
                SDL_DestroyTexture(texture.sdlTexture);
            }
            type = other.type;
            message = std::move(other.message);
            texture = other.texture;

            other.texture.sdlTexture = nullptr;
            other.texture.width = 0;
            other.texture.height = 0;
        }
        return *this;
    }
};

extern std::deque<LogEntry> AllLogs;
extern const size_t MAX_LOGS;
extern ThreadedQueue<LogEntry> PendingLogs;

SDL_Color chooseColor(LogType type);
const char* getTerminalColor(LogType type);
void gameLog(const char* msg, LogType type);
void gameLog(const std::string& msg, LogType type);
void renderLog();
void clearAllLogs();