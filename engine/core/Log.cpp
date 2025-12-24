#include "Log.h"
#include "Engine.h"
#include "render/TextureManager.h"
#include <deque>
#include <iostream>
#include <string>

std::deque<LogEntry> AllLogs;
const size_t MAX_LOGS = 35;

SDL_Color chooseColor(const LogType type) {
    switch (type) {
    case ERROR:   return {255, 100, 100, 255};
    case WARNING: return {255, 150, 100, 255};
    case INFO:    return {100, 100, 255, 255};
    case DEBUG:   return {255, 255, 100, 255};
    case PRINT:   return {255, 255, 255, 255};
    default:      return {255, 255, 255, 255};
    }
}

// TODO: fix V-RAM memory leak.
void gameLog(const char* msg, LogType type) {
    std::string prefix;
    switch (type) {
    case ERROR:   prefix = "[ERROR] "; break;
    case WARNING: prefix = "[WARNING] "; break;
    case INFO:    prefix = "[INFO] "; break;
    case DEBUG:   prefix = "[DEBUG] "; break;
    case PRINT:   prefix = ""; break;
    default:      prefix = "[UNKNOWN] "; break;
    }

    std::string combinedMsg = prefix + msg;
    std::cout << combinedMsg << std::endl;

    SDL_Texture* texture = nullptr;
    if (!renderer) {
        texture = nullptr;
    } else {
        texture = createTextureWithText(combinedMsg.c_str(), renderer, chooseColor(type));
    }

    AllLogs.push_back(LogEntry{type, combinedMsg, texture});

    while (AllLogs.size() > MAX_LOGS) {
        SDL_DestroyTexture(AllLogs.front().texture);
        AllLogs.pop_front();
    }
    std::cout << AllLogs.size();
}

void gameLog(const std::string& msg, const LogType type) {
    gameLog(msg.c_str(), type);
}

void clearAllLogs() {
    for (auto& entry : AllLogs) {
        if (entry.texture) {
            SDL_DestroyTexture(entry.texture);
            entry.texture = nullptr;
        }
    }
    AllLogs.clear();
}

void renderLog() {
    if (!renderer || !font) return;

    int width, height;
    SDL_GetWindowSize(window, &width, &height);

    for (size_t i = 0; i < AllLogs.size(); i++) {
        auto& entry = AllLogs[i];

        if (!entry.texture) {
            entry.texture = createTextureWithText(entry.message.c_str(), renderer, chooseColor(entry.type));
        }

        SDL_Rect rect;
        TTF_SizeText(font, entry.message.c_str(), &rect.w, &rect.h);
        rect.x = 20;
        rect.y = height - (i * (rect.h + 5) + 50);

        SDL_RenderCopy(renderer, entry.texture, nullptr, &rect);
    }
}