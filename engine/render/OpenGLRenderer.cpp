#include "OpenGLRenderer.h"
#include "core/Engine.h"
#include "core/Log.h"
#include "component/Sprite.h"
#include "component/Text.h"
#include "glad/glad.h"
#include "scene/GameObject.h"

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
    
    spriteShader = std::make_unique<Shader>("shaders/sprite.vert", "shaders/sprite.frag");
    if (spriteShader->ID == 0) {
        gameLog("Failed to create sprite shader", ERROR);
        return;
    }
    
    initQuad();
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    gameLog("OpenGLRenderer Initialized with texture support", INFO);
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void OpenGLRenderer::setupProjection() {
    float left = 0.0f;
    float right = static_cast<float>(screenWidth);
    float bottom = static_cast<float>(screenHeight);
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

void OpenGLRenderer::renderSprite(unsigned int textureID, float x, float y, float width, float height) {
    float model[16] = {
        width, 0.0f, 0.0f, 0.0f,
        0.0f, height, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        x, y, 0.0f, 1.0f
    };
    
    spriteShader->setMat4("model", model);
    spriteShader->setVec4("spriteColor", 1.0f, 1.0f, 1.0f, 1.0f);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    spriteShader->setInt("image", 0);
    
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void OpenGLRenderer::beginFrame() {
    glClearColor(0.05f, 0.1f, 0.08f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLRenderer::drawScene(std::vector<std::unique_ptr<GameObject>>& objects, const Camera& camera) {
    if (!spriteShader || objects.empty()) return;

    renderList.clear();
    renderList.reserve(objects.size());
    for (auto& obj : objects) {
        renderList.push_back(obj.get());
    }
    
    spriteShader->use();
    setupProjection();
    
    for (const auto* obj : renderList) {
        const auto sprite = obj->getComponent<Sprite>();
        if (sprite && sprite->texture.isValid() && sprite->texture.glTexture != 0) {
            float x = (obj->transform.position.x - camera.transform.position.x) * camera.zoom * 100.0f;
            float y = (camera.transform.position.y - obj->transform.position.y) * camera.zoom * 100.0f;
            float w = sprite->texture.width * obj->transform.scale.x * camera.zoom;
            float h = sprite->texture.height * obj->transform.scale.y * camera.zoom;
            
            x += screenWidth / 2.0f - w / 2.0f;
            y += screenHeight / 2.0f - h / 2.0f;
            
            renderSprite(sprite->texture.glTexture, x, y, w, h);
        }
        
        const auto text = obj->getComponent<Text>();
        if (text && text->texture.isValid() && text->texture.glTexture != 0) {
            float x = (obj->transform.position.x - camera.transform.position.x) * camera.zoom * 100.0f;
            float y = (camera.transform.position.y - obj->transform.position.y) * camera.zoom * 100.0f;
            float w = text->texture.width * camera.zoom;
            float h = text->texture.height * camera.zoom;
            
            x += screenWidth / 2.0f - w / 2.0f;
            y += screenHeight / 2.0f - h / 2.0f;
            
            renderSprite(text->texture.glTexture, x, y, w, h);
        }
    }
}

void OpenGLRenderer::endFrame() {
    SDL_GL_SwapWindow(window);
    dirtyList = false;
}