#pragma once
#include "SDL2/SDL.h"

extern SDL_Window* window;
extern SDL_Renderer* renderer;

int init();

int run(SDL_Renderer *renderer);

void quit();