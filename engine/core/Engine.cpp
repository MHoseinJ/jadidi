#include "Engine.h"

#include <iostream>
#include <thread>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>


SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

int init() {

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
        return 2;
    }

    if (TTF_Init() != 0) {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
        return 3;
    }

    SDL_Rect bounds;
    SDL_GetDisplayBounds(0, &bounds);

    window = SDL_CreateWindow(
        "jadidi",
        bounds.x,
        bounds.y,
        bounds.w,
        bounds.h,

        SDL_WINDOW_FULLSCREEN_DESKTOP

    );

    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return 4;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        return 5;
    }

    std::cout << bounds.x << " " << bounds.y << bounds.w << bounds.h << std::endl;

    return 0;
}

int run(SDL_Renderer *renderer) {

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        // TODO: draw stuff here

        SDL_RenderPresent(renderer);

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 0;
}

void quit () {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}