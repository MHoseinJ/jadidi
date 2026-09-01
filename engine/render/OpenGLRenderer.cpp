#include "OpenGLRenderer.h"
#include "SDL_video.h"
#include "core/Engine.h"
#include "core/Log.h"
#include "core/Units.h"
#include "component/Sprite.h"
#include "component/Text.h"
#include "scene/GameObject.h"
#include "glad/glad.h"
#include <algorithm>
#include <cmath>

OpenGLRenderer::OpenGLRenderer(SDL_Renderer* renderer)
    : sdlRenderer(renderer), dirtyList(true) {}

OpenGLRenderer::~OpenGLRenderer() {
    if (quadVAO != 0) glDeleteVertexArrays(1, &quadVAO);
    if (quadVBO != 0) glDeleteBuffers(1, &quadVBO);
    spriteShader.reset();
}

void OpenGLRenderer::init() {
    if (SDL_GL_MakeCurrent(window, SDL_GL_GetCurrentContext()) != 0) {
        gameLog("Failed to make GL context current: " + std::string(SDL_GetError()), ERROR);
        return;
    }
    
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        gameLog("Failed to load GL functions", ERROR);
        return;
    }
    
    SDL_GetWindowSize(window, &screenWidth, &screenHeight);
    
    spriteShader = std::make_unique<Shader>("Shaders/sprite.vert", "Shaders/sprite.frag");
    if (spriteShader->ID == 0) {
        gameLog("Failed to create sprite shader", ERROR);
        return;
    }
    
    initQuad();
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    gameLog("OpenGLRenderer Initialized successfully", INFO);
}

void OpenGLRenderer::addToBatch(unsigned int textureID, float x, float y, float w, float h,
                                 float uvOffX, float uvOffY, float uvScX, float uvScY) {
    if (textureID != currentBatchTexture && !batchVertices.empty()) {
        flushBatch();
    }
    currentBatchTexture = textureID;
    
    if (batchVertices.size() + 24 > MAX_BATCH_SPRITES * 24) {
        flushBatch();
    }
    
    batchVertices.insert(batchVertices.end(), {
        x,     y + h, uvOffX,          uvOffY + uvScY,
        x + w, y,     uvOffX + uvScX,  uvOffY,
        x,     y,     uvOffX,          uvOffY
    });
    
    batchVertices.insert(batchVertices.end(), {
        x,     y + h, uvOffX,          uvOffY + uvScY,
        x + w, y + h, uvOffX + uvScX,  uvOffY + uvScY,
        x + w, y,     uvOffX + uvScX,  uvOffY
    });
}

void OpenGLRenderer::flushBatch() {
    if (batchVertices.empty()) return;
    
    float identity[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    spriteShader->setMat4("model", identity);
    
    spriteShader->setVec2("uvOffset", 0.0f, 0.0f);
    spriteShader->setVec2("uvScale", 1.0f, 1.0f);
    spriteShader->setBool("useTexture", true);
    spriteShader->setVec4("spriteColor", 1.0f, 1.0f, 1.0f, 1.0f);
    
    glBindTexture(GL_TEXTURE_2D, currentBatchTexture);
    
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, batchVertices.size() * sizeof(float), batchVertices.data());
    
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(batchVertices.size() / 4));
    glBindVertexArray(0);
    
    batchVertices.clear();
}

void OpenGLRenderer::initQuad() {
    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 
        
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    
    glBufferData(GL_ARRAY_BUFFER, MAX_BATCH_SPRITES * 6 * 4 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void OpenGLRenderer::setupProjection() {
    float left = 0.0f;
    auto right = static_cast<float>(screenWidth);
    auto bottom = static_cast<float>(screenHeight);
    float top = 0.0f;
    float nearVal = -1.0f;
    float farVal = 1.0f;
    
    float proj[16] = {
        2.0f / (right - left), 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f / (top - bottom), 0.0f, 0.0f,
        0.0f, 0.0f, -2.0f / (farVal - nearVal), 0.0f,
        -(right + left) / (right - left), -(top + bottom) / (top - bottom), -(farVal + nearVal) / (farVal - nearVal), 1.0f
    };
    
    spriteShader->setMat4("projection", proj);
}

void OpenGLRenderer::renderColor(float x, float y, float width, float height, SDL_Color color) {
    float model[16] = {
        width, 0.0f, 0.0f, 0.0f,
        0.0f, height, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        x, y, 0.0f, 1.0f
    };
    
    spriteShader->setMat4("model", model);
    spriteShader->setVec4("spriteColor", 
        color.r / 255.0f, 
        color.g / 255.0f, 
        color.b / 255.0f, 
        color.a / 255.0f);
    spriteShader->setBool("useTexture", false);
    
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void OpenGLRenderer::sortObjectsByZIndex() {
    std::sort(renderList.begin(), renderList.end(),
              [](GameObject* a, GameObject* b) {
                  const auto aSprite = a->getComponent<Sprite>();
                  const auto bSprite = b->getComponent<Sprite>();
                  if (!aSprite && !bSprite) return false;
                  if (!aSprite) return true;
                  if (!bSprite) return false;
                  return aSprite->z_index < bSprite->z_index;
              });
}

void OpenGLRenderer::beginFrame() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLRenderer::drawScene(std::vector<std::unique_ptr<GameObject>>& objects, const Camera& camera) {
    if (!spriteShader || objects.empty()) return;
    
    renderList.clear();
    renderList.reserve(objects.size());
    for (auto& obj : objects) {
        renderList.push_back(obj.get());
    }
    
    if (dirtyList && !renderList.empty()) {
        sortObjectsByZIndex();
    }
    
    spriteShader->use();
    setupProjection();
    
    // draw sprites
    batchVertices.clear();
    currentBatchTexture = 0;
    
    for (const auto* obj : renderList) {
        const auto sprite = obj->getComponent<Sprite>();
        if (!sprite || sprite->srcRect.w <= 0 || sprite->srcRect.h <= 0)
            continue;
        
        const float w = sprite->srcRect.w * obj->transform.scale.x * camera.zoom;
        const float h = sprite->srcRect.h * obj->transform.scale.y * camera.zoom;
        
        float relX = (obj->transform.position.x - camera.transform.position.x) * camera.zoom * Units::PixelsPerMeter;
        float relY = (camera.transform.position.y - obj->transform.position.y) * camera.zoom * Units::PixelsPerMeter;
        
        float x = relX + (screenWidth / 2.0f) - (w / 2.0f);
        float y = relY + (screenHeight / 2.0f) - (h / 2.0f);
        
        if (sprite->hasTexture && sprite->texture.glTexture != 0) {
            float uvOffX = static_cast<float>(sprite->srcRect.x) / sprite->texture.width;
            float uvOffY = static_cast<float>(sprite->srcRect.y) / sprite->texture.height;
            float uvScX  = static_cast<float>(sprite->srcRect.w) / sprite->texture.width;
            float uvScY  = static_cast<float>(sprite->srcRect.h) / sprite->texture.height;
            
            addToBatch(sprite->texture.glTexture, x, y, w, h, uvOffX, uvOffY, uvScX, uvScY);
        } else {
            flushBatch();
            renderColor(x, y, w, h, sprite->color);
        }
    }
    flushBatch();
    
    // draw texts 
    batchVertices.clear();
    currentBatchTexture = 0;
    
    for (const auto* obj : renderList) {
        const auto text = obj->getComponent<Text>();
        if (!text || !text->texture.isValid() || text->srcRect.w <= 0 || text->srcRect.h <= 0)
            continue;
        
        if (text->texture.glTexture == 0) continue;
        
        const float w = text->srcRect.w * camera.zoom;
        const float h = text->srcRect.h * camera.zoom;
        
        float relX = (obj->transform.position.x - camera.transform.position.x) * camera.zoom * Units::PixelsPerMeter;
        float relY = (camera.transform.position.y - obj->transform.position.y) * camera.zoom * Units::PixelsPerMeter;
        
        float x = relX + (screenWidth / 2.0f) - (w / 2.0f);
        float y = relY + (screenHeight / 2.0f) - (h / 2.0f);
        
        float uvOffX = static_cast<float>(text->srcRect.x) / text->texture.width;
        float uvOffY = static_cast<float>(text->srcRect.y) / text->texture.height;
        float uvScX  = static_cast<float>(text->srcRect.w) / text->texture.width;
        float uvScY  = static_cast<float>(text->srcRect.h) / text->texture.height;
        
        addToBatch(text->texture.glTexture, x, y, w, h, uvOffX, uvOffY, uvScX, uvScY);
    }
    flushBatch();
}

void OpenGLRenderer::endFrame() {
    renderLog();
    SDL_GL_SwapWindow(window);
    dirtyList = false;
}

void OpenGLRenderer::renderLogs(int g_textures_created, int height) {
    spriteShader->use();
    setupProjection();
    
    batchVertices.clear();
    currentBatchTexture = 0;
    
    for (size_t i = 0; i < AllLogs.size(); i++) {
        auto& entry = AllLogs[i];
        
        if (!entry.texture.isValid()) {
            entry.texture = createTextureWithText(
                entry.message, renderer, chooseColor(entry.type), "font", 16
            );
            if (entry.texture.isValid()) ++g_textures_created;
            if (!entry.texture.isValid()) continue;
        }
        
        float w = entry.texture.width;
        float h = entry.texture.height;
        float y = height - (static_cast<int>(i) * (h + 5) + 50);
        float x = 25;
        
        if (entry.texture.glTexture) {
            addToBatch(entry.texture.glTexture, x, y, w, h, 0.0f, 0.0f, 1.0f, 1.0f);
        }
    }
    flushBatch();
}