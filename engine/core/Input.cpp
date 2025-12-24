#include "Input.h"
#include <cstring>

#include "Log.h"

bool Input::quitRequested = false;

bool Input::currentKeys[SDL_NUM_SCANCODES] = {0};
bool Input::previousKeys[SDL_NUM_SCANCODES] = {0};

bool Input::currentMouse[5] = {0};
bool Input::previousMouse[5] = {0};

Vector2 Input::currentMousePosition = {0, 0};

void Input::NewFrame() {
    std::memcpy(previousKeys, currentKeys, sizeof(currentKeys));
    std::memcpy(previousMouse, currentMouse, sizeof(currentMouse));
}

void Input::Update() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {

        switch (event.type) {

        case SDL_QUIT:
            quitRequested = true;
            break;

        case SDL_KEYDOWN: {
            if (event.key.repeat == 0)
                currentKeys[event.key.keysym.scancode] = true;
            break;
        }

        case SDL_KEYUP:
            currentKeys[event.key.keysym.scancode] = false;
            break;

        case SDL_MOUSEBUTTONDOWN:
            currentMouse[event.button.button] = true;
            break;

        case SDL_MOUSEBUTTONUP:
            currentMouse[event.button.button] = false;
            break;

        case SDL_MOUSEMOTION: {
            currentMousePosition.x = event.motion.x;
            currentMousePosition.y = event.motion.y;
            break;
        }
        default:
            break;
        }
    }
}

bool Input::IsKeyPressed(SDL_Scancode key) {
    return currentKeys[key] && !previousKeys[key];
}

bool Input::IsKeyDown(SDL_Scancode key) {
    return currentKeys[key];
}

bool Input::IsKeyUp(SDL_Scancode key) {
    return !currentKeys[key] && previousKeys[key];
}

bool Input::IsMouseButtonPressed(int button) {
    return currentMouse[button] && !previousMouse[button];
}

bool Input::IsMouseButtonDown(int button) {
    return currentMouse[button];
}

bool Input::IsMouseButtonUp(int button) {
    return !currentMouse[button] && previousMouse[button];
}

void Input::GetMousePosition(int& x, int& y) {
    x = currentMousePosition.x;
    y = currentMousePosition.y;
}

bool Input::QuitRequested() {
    return quitRequested;
}