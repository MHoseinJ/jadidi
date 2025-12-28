#pragma once
#include <SDL.h>
#include <string>
#include <unordered_map>
#include "utils/math/vector.h"

class Input {
public:
    // frame
    static void BeginFrame();
    static void Update();

    // number api
    static bool IsKeyPressed(SDL_Scancode code);
    static bool IsKeyDown(SDL_Scancode code);
    static bool IsKeyUp(SDL_Scancode code);

    static bool IsMouseButtonPressed(int button);
    static bool IsMouseButtonDown(int button);
    static bool IsMouseButtonUp(int button);

    // string overload
    static bool IsKeyPressed(const std::string& key);
    static bool IsKeyDown(const std::string& key);
    static bool IsKeyUp(const std::string& key);

    static bool IsMouseButtonPressed(const std::string& button);
    static bool IsMouseButtonDown(const std::string& button);
    static bool IsMouseButtonUp(const std::string& button);

    static void GetMousePosition(int& x, int& y);
    static void Quit();
    static bool QuitRequested();

private:
    static bool quitRequested;

    static bool currentKeys[SDL_NUM_SCANCODES];
    static bool previousKeys[SDL_NUM_SCANCODES];

    static bool currentMouse[8];
    static bool previousMouse[8];

    static Vector2 currentMousePosition;

    // string equivalent
    static std::unordered_map<std::string, SDL_Scancode> keyMap;
    static std::unordered_map<std::string, int> mouseMap;
};
