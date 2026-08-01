#pragma once

#include "core/IRenderer.h"
#include <SDL2/SDL.h>
#include "utils/math/vector.h"

class SDLRenderer : public IRenderer {
private:
    SDL_Renderer* sdlRenderer;
    Vector2 screenSize;
    bool dirtyList;

    void sortObjectsByZIndex(std::vector<std::unique_ptr<GameObject>>& objects);

public:
    explicit SDLRenderer(SDL_Renderer* renderer);
    ~SDLRenderer() override = default;

    void init() override;
    void beginFrame() override;
    void drawScene(std::vector<std::unique_ptr<GameObject>>& objects, const Camera& camera) override;
    void endFrame() override;

    void markDirty() { dirtyList = true; }
};