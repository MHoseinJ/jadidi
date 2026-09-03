#pragma once
#include "core/IRenderer.h"
#include "render/Shader.h"
#include <SDL2/SDL.h>
#include <memory>
#include <vector>

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
    static constexpr int MAX_BATCH_SPRITES = 2000;
    std::vector<float> batchVertices;
    unsigned int currentBatchTexture = 0;

    void addToBatch(unsigned int textureID, float x, float y, float w, float h, float uvOffX, float uvOffY, float uvScX,
                    float uvScY);
    void flushBatch();
    void initQuad();
    void setupProjection();
    void renderColor(float x, float y, float width, float height, SDL_Color color);
    void sortObjectsByZIndex();

  public:
    explicit OpenGLRenderer(SDL_Renderer* renderer);
    ~OpenGLRenderer() override;

    void init() override;
    void beginFrame() override;
    void drawScene(std::vector<std::unique_ptr<GameObject>>& objects, const Camera& camera) override;
    void endFrame() override;
    void renderLogs(int g_textures_created, int height) override;

    void markDirty() override {
        dirtyList = true;
    }
};