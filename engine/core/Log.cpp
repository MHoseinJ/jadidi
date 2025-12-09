#include "Log.h"
#include <iostream>
#include "Engine.h"
#include "render/TextureManager.h"
#include <cstring>

std::deque<LogEntry> AllLogs;

SDL_Color chooseColor(const LogType type) {
    SDL_Color color;
    switch (type) {
        case ERROR:
            color = SDL_Color { 255, 100, 100, 255 };
            break;
        case WARNING:
            color = SDL_Color { 255, 150, 100, 255 };
            break;
        case INFO:
            color = SDL_Color { 100, 100, 255, 255 };
            break;
        case DEBUG:
            color = SDL_Color { 255, 255, 100, 255 };
            break;
        default:
            color = SDL_Color { 255, 255, 255, 255 };
    }

    return color;
}

void gameLog(const char* msg, const LogType type) {
    SDL_Color color = chooseColor(type);

    const char* prefix =
        (type == ERROR)   ? "[ERROR] " :
        (type == WARNING) ? "[WARNING] " :
        (type == INFO)    ? "[INFO] " :
        (type == DEBUG)   ? "[DEBUG] " :
                            "[UNKNOWN] ";

    std::string combinedMsg = std::string(prefix) + msg;

    std::cout << combinedMsg << std::endl;

    SDL_Texture* texture = nullptr;
    if (renderer) {
        texture = createTextureWithText(combinedMsg.c_str(), renderer, color);
    }

    AllLogs.push_back(LogEntry{type, combinedMsg, texture});

    if (AllLogs.size() > 20) {
        SDL_DestroyTexture(AllLogs.front().texture);
        AllLogs.pop_front();
    }
}

void renderLog() {
    int width, height;
    SDL_GetWindowSize(window, &width, &height);

    for (size_t i = 0; i < AllLogs.size(); i++) {
        auto& entry = AllLogs[i];

        SDL_Rect rect;
        TTF_SizeText(font, entry.message.c_str(), &rect.w, &rect.h);
        rect.x = 20;
        rect.y = height - (i * (rect.h + 5) + 50);

        if (entry.texture == nullptr) {
            entry.texture = createTextureWithText(
                entry.message.c_str(), renderer, chooseColor(entry.type)
            );
        }

        SDL_RenderCopy(renderer, entry.texture, nullptr, &rect);
    }
}
