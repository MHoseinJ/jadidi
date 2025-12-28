#pragma once

#include "SDL_render.h"
#include "SDL_video.h"

extern SDL_Window* window;
extern SDL_Renderer* renderer;

int init();

void run();

void quit();
