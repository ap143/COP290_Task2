#ifndef DRAW
#define DRAW

#include "SDL.h"
#include "SDL_ttf.h"
#include <iostream>
#include "helper.hpp"

void line(SDL_Renderer* renderer, float x1, float y1, float x2, float y2);

void point(SDL_Renderer* renderer, float x, float y);

void rect(SDL_Renderer* renderer, float x, float y, float width, float height, bool filled);

void text(SDL_Renderer* renderer, std::string text_to_display, float x, float y, unsigned int size);

void text(SDL_Renderer* renderer, std::string text_to_display, float x, float y);

#endif