#include "OpenGLRenderer.h"
#include "core/Engine.h"
#include <iostream>

OpenGLRenderer::OpenGLRenderer(SDL_Renderer* renderer) 
    : sdlRenderer(renderer), dirtyList(true) {}

void OpenGLRenderer::init() {
    
    if (SDL_GL_MakeCurrent(window, SDL_GL_GetCurrentContext()) != 0) {
        std::cerr << "Failed to make GL context current: " << SDL_GetError() << std::endl;
    }
    
    std::cout << "[OpenGLRenderer] Initialized successfully." << std::endl;
}

void OpenGLRenderer::beginFrame() {
    // TODO: add clear screen
}

void OpenGLRenderer::drawScene(std::vector<GameObject>& objects, const Camera& camera) {    
    // TODO: add gl render
    if (objects.empty()) return;
}

void OpenGLRenderer::endFrame() {
    SDL_GL_SwapWindow(window);
    dirtyList = false;
}