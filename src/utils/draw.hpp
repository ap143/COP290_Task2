#ifndef DRAW
#define DRAW

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include <iostream>
#include "helper.hpp"

void color(SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
void color(SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b);
void color(SDL_Renderer* renderer, Uint8 grey, Uint8 alpha);
void color(SDL_Renderer* renderer, Uint8 grey);

void line(SDL_Renderer *renderer, float x1, float y1, float x2, float y2);

void point(SDL_Renderer *renderer, float x, float y);

void rect(SDL_Renderer *renderer, float x, float y, float width, float height, bool filled);
void rectCenter(SDL_Renderer* renderer, float cx, float cy, float width, float height, float scale, bool filled);

SDL_Texture* text(SDL_Renderer *renderer, std::string text_to_display, unsigned int size);

SDL_Texture* text(SDL_Renderer *renderer, std::string text_to_display);

SDL_Texture *loadTexture(const char *texture, SDL_Renderer *ren);

void imageCenter(SDL_Renderer *renderer, SDL_Texture* texture, float x, float y);
void image(SDL_Renderer *renderer, SDL_Texture *texture, float sx, float sy, float sw, float sh, float dx,
           float dy, float dw, float dh, float angle, SDL_RendererFlip flip);
void image(SDL_Renderer *renderer, SDL_Texture *texture, float dx,
           float dy, float dw, float dh, float angle, SDL_RendererFlip flip);
void image(SDL_Renderer *renderer, SDL_Texture *texture, float dx,
           float dy, float angle, SDL_RendererFlip flip);
void image(SDL_Renderer *renderer, SDL_Texture *texture, float angle, SDL_RendererFlip flip);

void backgroundImage(SDL_Renderer *renderer, SDL_Texture *texture);

SDL_FRect getSDLFRect(float x, float y, float width, float height);

SDL_Rect getSDLRect(float x, float y, float width, float height);

void loadSpritesTexture(SDL_Texture* spriteSheet, SDL_Texture texs[4][3]);

#endif