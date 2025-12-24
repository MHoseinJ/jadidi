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
};

extern std::deque<LogEntry> AllLogs;

SDL_Color chooseColor(LogType type);
void gameLog(const char* msg, LogType type);
void gameLog(const std::string& msg, const LogType type);
void renderLog();
void clearAllLogs();