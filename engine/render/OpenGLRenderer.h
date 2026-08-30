#pragma once
#include "core/IRenderer.h"
#include "render/Shader.h"
#include <SDL2/SDL.h>
#include <vector>
#include <memory>

class GameObject;
class Camera;

class OpenGLRenderer : public IRenderer {
private:
    SDL_Renderer* sdlRenderer;
    bool dirtyList;
    
    std::unique_ptr<Shader> spriteShader;
    unsigned int quadVAO = 0;
    unsigned int quadVBO = 0;

    std::vector<GameObject*> renderList;
    
    int screenWidth = 0;
    int screenHeight = 0;
    
    void initQuad();
    void setupProjection();
    void renderSprite(unsigned int textureID, float x, float y, float width, float height);

public:
    explicit OpenGLRenderer(SDL_Renderer* renderer);
    ~OpenGLRenderer() override;
    
    void init() override;
    void beginFrame() override;
    void drawScene(std::vector<std::unique_ptr<GameObject>>& objects, const Camera& camera) override;
    void endFrame() override;
    void markDirty() override { dirtyList = true; }
};