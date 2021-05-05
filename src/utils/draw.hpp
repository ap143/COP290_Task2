#ifndef DRAW
#define DRAW

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include <iostream>
#include "helper.hpp"

#define TEAM_SELECT "00"
#define MAZE_STRUCT "01"
#define READY "02"
#define DEPLOY "03"
#define MOVEMENT "04"
#define ATTACK "05"
#define DIE "06"
#define TURN "07"
#define END_GAME "08"


void color(SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
void color(SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b);
void color(SDL_Renderer* renderer, Uint8 grey, Uint8 alpha);
void color(SDL_Renderer* renderer, Uint8 grey);

void line(SDL_Renderer *renderer, float x1, float y1, float x2, float y2);

void point(SDL_Renderer *renderer, float x, float y);

void rect(SDL_Renderer *renderer, float x, float y, float width, float height, bool filled);
void rectCenter(SDL_Renderer* renderer, float cx, float cy, float width, float height, float scale, bool filled);

SDL_Texture* text(SDL_Renderer *renderer, std::string text_to_display, float size);

SDL_Texture* text(SDL_Renderer *renderer, std::string text_to_display);

SDL_Texture *loadTexture(const char *texture, SDL_Renderer *ren);

void imageCenter(SDL_Renderer *renderer, SDL_Texture* texture, float x, float y);

void imageCenter(SDL_Renderer *renderer, SDL_Texture* texture, SDL_Rect* src, float x, float y, float width, float height);

void imageCenter(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *src, float x, float y, float width, float height, float angle, SDL_RendererFlip flip);

void imageCenter(SDL_Renderer *renderer, SDL_Texture* texture, SDL_Rect* src, float x, float y, float width, float height, float scale);

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