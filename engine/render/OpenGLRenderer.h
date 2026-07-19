#pragma once

#include "core/IRenderer.h"
#include <SDL2/SDL.h>
#include <vector>

class GameObject;
class Camera;

class OpenGLRenderer : public IRenderer {
private:
    SDL_Renderer* sdlRenderer;
    bool dirtyList;

public:
    explicit OpenGLRenderer(SDL_Renderer* renderer);
    ~OpenGLRenderer() override = default;

    void init() override;
    void beginFrame() override;
    void drawScene(std::vector<GameObject>& objects, const Camera& camera) override;
    void endFrame() override;
    void markDirty() override { dirtyList = true; }
};