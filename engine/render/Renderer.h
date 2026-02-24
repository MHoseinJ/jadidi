#pragma once
#include <SDL_render.h>
#include <vector>
#include "scene/GameObject.h"
#include "component/Component.h"

inline bool dirtyList = true;
inline void markDirty() { dirtyList = true; }

struct Camera;

void drawObjects(SDL_Renderer* renderer, std::vector<GameObject>& objects, const Camera& camera);
void drawObjects(SDL_Renderer* renderer, std::nullptr_t);