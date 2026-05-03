#pragma once

#include "SDL_render.h"
#include "SDL_video.h"

extern SDL_Window* window;
extern SDL_Renderer* renderer;

// init window, audio, graphics and stuff
int init();

// main loop containing scripting loop and scene management and graphics
void run();

// cleanup engine and ready to exit
void quit();
