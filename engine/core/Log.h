#pragma once
#include <deque>
#include <SDL_render.h>
#include <string>

enum LogType {
    ERROR,
    WARNING,
    INFO,
    DEBUG,
};

struct LogEntry {
    LogType type;
    std::string message;
    SDL_Texture* texture = nullptr;
};

extern std::deque<LogEntry> AllLogs;

SDL_Color chooseColor(LogType type);
void gameLog(const char* msg, LogType type);
void renderLog();