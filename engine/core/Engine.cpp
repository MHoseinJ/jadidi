#include "Engine.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "Input.h"
#include "Log.h"
#include "Timer.h"
#include "component/Factory.h"

#include "lua/LuaBindings.h"
#include "render/FontManager.h"
#include "render/Renderer.h"
#include "render/TextureManager.h"
#include "scene/SceneManager.h"
#include "utils/Config.h"
#include "scene/GameObject.h"
#include "scene/UI.h"

SDL_Window* window   = nullptr;
SDL_Renderer* renderer = nullptr;

int init() {
    
    Config cfg("config.json");

    if (!cfg.load()) {
        gameLog("config.json not found or corrupted. Creating default one.", WARNING);

        cfg.data() = {
            { "window", {
                { "title", "jadidi" },
                { "fullscreen", true },
                { "width", 1280 },
                { "height", 720 },
                { "icon", "icon.bmp" }
            }}
        };
        cfg.save();
    }

    auto windowCfg = cfg.data()["window"];

    const bool        fullscreen = windowCfg.value("fullscreen", true);
    const std::string title      = windowCfg.value("title", "jadidi");
    const std::string iconPath   = windowCfg.value("icon", "icon.bmp");
    int               width      = windowCfg.value("width", 1280);
    int               height     = windowCfg.value("height", 720);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        gameLog(std::string("video initialize error: ") + SDL_GetError(), ERROR);
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        gameLog(std::string("IMG_Init Error: ") + IMG_GetError(), ERROR);
        return 2;
    }

    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        gameLog(std::string("audio initialize error: ") + SDL_GetError(), ERROR);
        return 3;
    }

    if (initTTF() != 0) {
        gameLog(std::string("TTF initialize error: ") + TTF_GetError(), ERROR);
        // TODO: should this return an error code too?
    }

    Uint32 flags = fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0;

    if (fullscreen) {
        SDL_Rect bounds;
        SDL_GetDisplayBounds(0, &bounds);
        width  = bounds.w;
        height = bounds.h;
    }

    window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        flags
    );

    if (!window) {
        gameLog(std::string("SDL_CreateWindow Error: ") + SDL_GetError(), ERROR);
        return 4;
    }

    if (SDL_Surface* icon = SDL_LoadBMP(iconPath.c_str())) {
        SDL_SetWindowIcon(window, icon);
        SDL_FreeSurface(icon);
    } else {
        gameLog(std::string("failed to load icon '") + iconPath + "': " + SDL_GetError(), WARNING);
    }

    renderer = SDL_CreateRenderer(
        window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!renderer) {
        gameLog(std::string("SDL_CreateRenderer Error: ") + SDL_GetError(), ERROR);
        return 5;
    }

    // Use the configured logical resolution (not the output size!)
    SDL_RenderSetLogicalSize(renderer, width, height);
    SDL_RenderSetIntegerScale(renderer, SDL_TRUE);

    initRenderer();

    gameLog("GameEngine fully initialized", INFO);
    return 0;
}

void run() {
    registerComponents();
    bool running = true;

    Timer::initTimer();

    Lua::loadSceneScripts("home");

    SceneManager::getInstance().loadScene("home");

    Lua::callStartLua();

    Scene& gameScene = SceneManager::getInstance().getCurrentScene();

    while (running) {
        Input::BeginFrame();
        Input::Update();

        if (Input::QuitRequested()) running = false;

        const float dt = Timer::deltaTime();

        for (auto& obj : gameScene.objects) obj.Update(dt);

        Lua::callUpdateLua(dt);

        UIManager::getInstance()->Update();

        drawObjects(renderer, gameScene.objects, camera);
    }

}

void quit() {
    FontManager::instance().clean();
    clearAllLogs();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}
