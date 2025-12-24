#pragma once
#include <SDL.h>
#include "utils/math/vector.h"

class Input {
public:
    static void NewFrame();
    static void Update();

    static bool IsKeyPressed(SDL_Scancode key);
    static bool IsKeyDown(SDL_Scancode key);
    static bool IsKeyUp(SDL_Scancode key);

    static bool IsMouseButtonPressed(int button);
    static bool IsMouseButtonDown(int button);
    static bool IsMouseButtonUp(int button);

    static void GetMousePosition(int& x, int& y);

    static bool QuitRequested();

private:
    static bool quitRequested;

    static bool currentKeys[SDL_NUM_SCANCODES];
    static bool previousKeys[SDL_NUM_SCANCODES];

    static bool currentMouse[5];
    static bool previousMouse[5];

    static Vector2 currentMousePosition;
};