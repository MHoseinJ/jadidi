#pragma once
#include <SDL_render.h>
#include <deque>
#include <string>

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
    SDL_Texture* texture = nullptr;

    LogEntry(LogType t, std::string&& msg, SDL_Texture* tex = nullptr)
        : type(t), message(std::move(msg)), texture(tex) {}

    ~LogEntry() {
        if (texture) {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }
    }

    LogEntry(const LogEntry&) = delete;
    LogEntry& operator=(const LogEntry&) = delete;

    LogEntry(LogEntry&& other) noexcept
        : type(other.type),
          message(std::move(other.message)),
          texture(other.texture)
    {
        other.texture = nullptr;
    }

    LogEntry& operator=(LogEntry&& other) noexcept {
        if (this != &other) {
            if (texture) {
                SDL_DestroyTexture(texture);
            }
            type = other.type;
            message = std::move(other.message);
            texture = other.texture;
            other.texture = nullptr;
        }
        return *this;
    }
};

extern std::deque<LogEntry> AllLogs;
extern const size_t MAX_LOGS;

SDL_Color chooseColor(LogType type);
void gameLog(const char* msg, LogType type);
void gameLog(const std::string& msg, const LogType type);
void renderLog();
void clearAllLogs();