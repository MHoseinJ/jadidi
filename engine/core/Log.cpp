#include "Log.h"
#include "Engine.h"
#include "render/TextureManager.h"
#include <iostream>
#include <atomic>
#include <mutex>

ThreadedQueue<LogEntry> PendingLogs;
std::deque<LogEntry> AllLogs;
const size_t MAX_LOGS = 35;

std::atomic<int> g_textures_created{0};
std::atomic<int> g_textures_destroyed{0};

std::mutex allLogsMutex;

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

const char* getTerminalColor(LogType type) {
    switch (type) {
        case ERROR:   return "\033[1;31m";
        case WARNING: return "\033[1;33m";
        case INFO:    return "\033[1;34m";
        case DEBUG:   return "\033[1;36m";
        case PRINT:   return "\033[0m";
        default:      return "\033[0m";
    }
}

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
    std::cout << getTerminalColor(type) << combinedMsg << "\033[0m" << std::endl;

    PendingLogs.emplace(type, std::move(combinedMsg), nullptr);
}

void gameLog(const std::string& msg, const LogType type) {
    gameLog(msg.c_str(), type);
}

void clearAllLogs() {
    {
        std::lock_guard<std::mutex> lock(allLogsMutex);
        AllLogs.clear();
    }
    gameLog("all logs cleared", INFO);
}

void renderLog() {
    if (!renderer) {
        return;
    }

    int width, height;
    SDL_GetWindowSize(window, &width, &height);

    {
        std::lock_guard<std::mutex> lock(allLogsMutex);
        LogEntry entry;

        while (PendingLogs.tryPop(entry)) {
            AllLogs.emplace_back(std::move(entry));
        }

        while (AllLogs.size() > MAX_LOGS) {
            AllLogs.pop_front();
        }
    }

    std::lock_guard<std::mutex> lock(allLogsMutex);
    
    for (size_t i = 0; i < AllLogs.size(); i++) {
        auto& entry = AllLogs[i];

        if (!entry.texture) {
            entry.texture = createTextureWithText(entry.message.c_str(), renderer, chooseColor(entry.type), "font", 16);
            if (entry.texture) ++g_textures_created;
        }

        SDL_Rect rect;
        SDL_QueryTexture(entry.texture, nullptr, nullptr, &rect.w, &rect.h);
        rect.y = height - (static_cast<int>(i) * (rect.h + 5) + 50);
        rect.x = 25;

        if (entry.texture) {
            SDL_RenderCopy(renderer, entry.texture, nullptr, &rect);
        }
    }
}