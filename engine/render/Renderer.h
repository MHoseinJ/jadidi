#pragma once
#include <SDL_render.h>
#include <vector>
#include "scene/GameObject.h"

inline bool dirtyList = true;

inline void isListDirty() {
    dirtyList = true;
}

void drawObjects(SDL_Renderer* renderer, std::vector<GameObject>& objects, Camera& camera);

void drawObjects(SDL_Renderer* renderer, std::nullptr_t);