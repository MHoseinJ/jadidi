#pragma once

#include "core/IRenderer.h"
#include "scene/GameObject.h"
#include "utils/math/vector.h"
#include <SDL2/SDL.h>

class SDLRenderer : public IRenderer {
  private:
    SDL_Renderer* sdlRenderer;
    Vector2 screenSize;
    bool dirtyList;

    std::vector<GameObject*> renderList;

    void sortObjectsByZIndex();

  public:
    explicit SDLRenderer(SDL_Renderer* renderer);
    ~SDLRenderer() override = default;

    void init() override;
    void beginFrame() override;
    void drawScene(std::vector<std::unique_ptr<GameObject>>& objects, const Camera& camera) override;
    void endFrame() override;
    void renderLogs(int g_textures_created, int height) override;

    void markDirty() override {
        dirtyList = true;
    }
};