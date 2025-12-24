#include "Timer.h"
#include <SDL_timer.h>

int Timer::lastFrame = 0;

void Timer::initTimer() {
    lastFrame = SDL_GetTicks();
}

float Timer::deltaTime() {
    int now = SDL_GetTicks();
    float delta = (now - lastFrame) / 1000.0f;

    if (delta < 0.000001f)
        delta = 0.000001f;

    if (delta > 0.1f)
        delta = 0.1f;

    lastFrame = now;
    return delta;
}

float Timer::frameRate() {
    return 1.0f / deltaTime();
}