#include "Engine.h"

#include <fstream>
#include <iostream>
#include <thread>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "Log.h"
#include "render/Renderer.h"
#include "render/TextureManager.h"
#include "utils/FileSystem.h"
#include "utils/Config.h"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

int init() {

    Config cfg("config.json");
    if (!cfg.load()) {
        gameLog("config.json not found or corrupted. Creating default one.", WARNING);

        cfg.data() = {
            {"window", {
                {"title", "jadidi"},
                {"fullscreen", true},
                {"width", 1280},
                {"height", 720}
            }}
        };

        cfg.save();
    }

    auto windowCfg = cfg.data()["window"];

    bool fullscreen = windowCfg.value("fullscreen", true);
    int width      = windowCfg.value("width", 1280);
    int height     = windowCfg.value("height", 720);
    std::string title = windowCfg.value("title", "jadidi");

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        gameLog((std::string("video initialize error: ") + SDL_GetError()).c_str(), ERROR);
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        gameLog((std::string("IMG_Init Error: ") + IMG_GetError()).c_str(), ERROR);
        return 2;
    }

    if (TTF_Init() != 0) {
        gameLog((std::string("TTF_Init Error: ") + TTF_GetError()).c_str(), ERROR);
        return 3;
    }

    if (initFont() != 0) {
        gameLog("initFont Error", WARNING);
    }

    Uint32 flags = 0;

    if (fullscreen)
        flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

    if (fullscreen) {
        SDL_Rect bounds;
        SDL_GetDisplayBounds(0, &bounds);
        width = bounds.w;
        height = bounds.h;
    }

    window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        flags
    );

    if (!window) {
        gameLog((std::string("SDL_CreateWindow Error: ") + SDL_GetError()).c_str(), ERROR);
        return 4;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        gameLog((std::string("SDL_CreateRenderer Error: ") + SDL_GetError()).c_str(), ERROR);
        return 5;
    }

    gameLog("GameEngine fully initialized", INFO);
    return 0;
}

void run() {

    bool running = true;
    SDL_Event event;

    gameLog("warning bro", WARNING);

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        drawObjects(renderer, nullptr);

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

}

void quit () {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}