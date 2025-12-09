#pragma once
#include <SDL_render.h>
#include <SDL_ttf.h>
#include <string>

extern TTF_Font *font;

int initFont();

SDL_Texture* createTextureWithText(const std::string& text, SDL_Renderer* renderer, SDL_Color color);