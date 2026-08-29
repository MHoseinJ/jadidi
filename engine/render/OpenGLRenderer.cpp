#include "OpenGLRenderer.h"
#include "core/Engine.h"
#include "core/Log.h"
#include "glad/glad.h"

OpenGLRenderer::OpenGLRenderer(SDL_Renderer* renderer) 
    : sdlRenderer(renderer) {}

OpenGLRenderer::~OpenGLRenderer() {
    if (VAO != 0) glDeleteVertexArrays(1, &VAO);
    if (VBO != 0) glDeleteBuffers(1, &VBO);
    shader.reset();
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

    shader = std::make_unique<Shader>("shaders/triangle.vert", "shaders/triangle.frag");
    if (shader->ID == 0) {
        gameLog("Failed to create triangle shader", ERROR);
        return;
    }

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    gameLog("OpenGLRenderer Initialized with simple triangle shader", INFO);
}

void OpenGLRenderer::beginFrame() {
    glClearColor(0.05f, 0.1f, 0.08f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLRenderer::drawScene(std::vector<std::unique_ptr<GameObject>>& objects, const Camera& camera) {
    if (!shader) return;

    shader->use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void OpenGLRenderer::endFrame() {
    SDL_GL_SwapWindow(window);
}