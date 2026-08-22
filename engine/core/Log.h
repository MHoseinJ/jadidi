#pragma once
#include <SDL2/SDL_render.h>
#include <deque>
#include <string>
#include "container/Queue.h"
#include "render/TextureHandle.h"

// Forward declarations
class ITextureBackend;
class TextureManager;

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

    ~LogEntry();

    LogEntry(const LogEntry&) = delete;
    LogEntry& operator=(const LogEntry&) = delete;

    LogEntry(LogEntry&& other) noexcept;
    LogEntry& operator=(LogEntry&& other) noexcept;
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