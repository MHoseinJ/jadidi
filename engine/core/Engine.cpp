#include "Engine.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

int init() {

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Rect bounds;
    SDL_GetDisplayBounds(0, &bounds);

    window = SDL_CreateWindow(
        'jadidi',
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        bounds.x,
        bounds.y,

        SDL_WINDOW_FULLSCREEN_DESKTOP

    );

    return 0;
}
