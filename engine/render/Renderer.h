#pragma once
#include <SDL_render.h>
#include <vector>
#include "scene/GameObject.h"

void drawObjects(SDL_Renderer* renderer, const std::vector<GameObject>& objects, Camera& camera);

void drawObjects(SDL_Renderer* renderer, std::nullptr_t);