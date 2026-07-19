#pragma once

#include "core/IRenderer.h"
#include <SDL2/SDL.h>
#include "utils/math/vector.h"

class SDLRenderer : public IRenderer {
private:
    SDL_Renderer* sdlRenderer;
    Vector2 screenSize;
    int meterSizeInPixels;
    bool dirtyList;

    void sortObjectsByZIndex(std::vector<GameObject>& objects);

public:
    explicit SDLRenderer(SDL_Renderer* renderer);
    ~SDLRenderer() override = default;

    void init() override;
    void beginFrame() override;
    void drawScene(std::vector<GameObject>& objects, const Camera& camera) override;
    void endFrame() override;

    void markDirty() { dirtyList = true; }
};