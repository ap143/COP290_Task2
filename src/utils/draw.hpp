#ifndef DRAW
#define DRAW

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include <iostream>
#include "helper.hpp"

void line(SDL_Renderer *renderer, float x1, float y1, float x2, float y2);

void point(SDL_Renderer *renderer, float x, float y);

void rect(SDL_Renderer *renderer, float x, float y, float width, float height, bool filled);

void text(SDL_Renderer *renderer, std::string text_to_display, float x, float y, unsigned int size);

void text(SDL_Renderer *renderer, std::string text_to_display, float x, float y);

SDL_Texture *loadTexture(const char *texture, SDL_Renderer *ren);

void image(SDL_Renderer *renderer, SDL_Texture *texture, float sx, float sy, float sw, float sh, float dx,
           float dy, float dw, float dh, float angle, SDL_RendererFlip flip);

void image(SDL_Renderer *renderer, SDL_Texture *texture, float dx,
           float dy, float dw, float dh, float angle, SDL_RendererFlip flip);

void image(SDL_Renderer *renderer, SDL_Texture *texture, float angle, SDL_RendererFlip flip);

void backgroundImage(SDL_Renderer *renderer, SDL_Texture *texture);

SDL_FRect getSDLFRect(float x, float y, float width, float height);

SDL_Rect getSDLRect(float x, float y, float width, float height);

#endif