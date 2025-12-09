#pragma once
#include <SDL_render.h>
#include <vector>

void drawObjects(SDL_Renderer* renderer, const std::vector<SDL_Texture*>& textures);

void drawObjects(SDL_Renderer* renderer, std::nullptr_t);