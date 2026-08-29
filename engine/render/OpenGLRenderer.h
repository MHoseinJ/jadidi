#pragma once
#include "core/IRenderer.h"
#include "render/Shader.h"
#include <SDL2/SDL.h>
#include <memory>

class GameObject;
class Camera;

class OpenGLRenderer : public IRenderer {
private:
    SDL_Renderer* sdlRenderer;
    std::unique_ptr<Shader> shader;
    unsigned int VAO = 0;
    unsigned int VBO = 0;

public:
    explicit OpenGLRenderer(SDL_Renderer* renderer);
    ~OpenGLRenderer() override;
    
    void init() override;
    void beginFrame() override;
    void drawScene(std::vector<std::unique_ptr<GameObject>>& objects, const Camera& camera) override;
    void endFrame() override;
    void markDirty() override {}
};