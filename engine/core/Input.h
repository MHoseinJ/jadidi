#pragma once
#include <SDL.h>
#include <unordered_map>
#include <string>
#include "utils/math/vector.h"

class Input {
public:
    static void BeginFrame();
    static void Update();

    static bool IsKeyPressed(const std::string& keyName);
    static bool IsKeyDown(const std::string& keyName);
    static bool IsKeyUp(const std::string& keyName);

    static bool IsMouseButtonPressed(const std::string& buttonName);
    static bool IsMouseButtonDown(const std::string& buttonName);
    static bool IsMouseButtonUp(const std::string& buttonName);

    static void GetMousePosition(int& x, int& y);
    static bool QuitRequested();

private:
    static bool quitRequested;

    static bool currentKeys[SDL_NUM_SCANCODES];
    static bool previousKeys[SDL_NUM_SCANCODES];

    static bool currentMouse[5];
    static bool previousMouse[5];

    static Vector2 currentMousePosition;

    static std::unordered_map<std::string, SDL_Scancode> keyMap;
    static std::unordered_map<std::string, int> mouseMap;

    static SDL_Scancode GetScancode(const std::string& keyName);
    static int GetMouseButton(const std::string& buttonName);
};