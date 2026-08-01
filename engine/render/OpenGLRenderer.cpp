#include "OpenGLRenderer.h"
#include "core/Engine.h"
#include "core/Log.h"
#include <iostream>
#include <string>

OpenGLRenderer::OpenGLRenderer(SDL_Renderer* renderer) 
    : sdlRenderer(renderer), dirtyList(true) {}

void OpenGLRenderer::init() {
    
    if (SDL_GL_MakeCurrent(window, SDL_GL_GetCurrentContext()) != 0) {
        gameLog("Failed to make GL context current: " + std::string(SDL_GetError()), ERROR);
    }
    
    gameLog("OpenGLRenderer Initialized successfully.", INFO);
}

void OpenGLRenderer::beginFrame() {
    // TODO: add clear screen
}

void OpenGLRenderer::drawScene(std::vector<std::unique_ptr<GameObject>>& objects, const Camera& camera) {    
    // TODO: add gl render
    if (objects.empty()) return;
}

void OpenGLRenderer::endFrame() {
    SDL_GL_SwapWindow(window);
    dirtyList = false;
}