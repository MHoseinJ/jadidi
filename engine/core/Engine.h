#pragma once

#include "core/IRenderer.h"
#include "physics/Wrapper.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <memory>
#include <optional>

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern std::unique_ptr<IRenderer> rendererInterface;
extern std::optional<Physics> physics;

// init window, audio, graphics and stuff
int init();

// main loop containing scripting loop and scene management and graphics
void run();

// cleanup engine and ready to exit
void quit();