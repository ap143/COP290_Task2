#include "draw.hpp"

// Variables to store current color of the renderer
// Change these values in setColor()
Uint8 _r = 0;
Uint8 _g = 0;
Uint8 _b = 0;
Uint8 _a = 255;

void line(SDL_Renderer* renderer, float x1, float y1, float x2, float y2)
{
    SDL_RenderDrawLineF(renderer, x1, y1, x2, y2);
}

void point(SDL_Renderer* renderer, float x, float y)
{
    SDL_RenderDrawPointF(renderer, x, y);
}

void rect(SDL_Renderer* renderer, float x, float y, float width, float height, bool filled)
{
    SDL_FRect rect = {.x = x, .y = y, .w = width, .h = height};
    if (filled)
    {
        SDL_RenderFillRectF(renderer, &rect);
    }
    else
    {
        SDL_RenderDrawRectF(renderer, &rect);
    }
}

void text(SDL_Renderer* renderer, std::string text_to_display, float x, float y, unsigned int size)
{
    TTF_Font* font = TTF_OpenFont("./assets/fonts/CONSOLAB.TTF", size);
    SDL_Color fontColor = {.r = _r, .g = _g, .b = _b, .a = _a};
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text_to_display.c_str(), fontColor);
    SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, textSurface);
    int text_width = textSurface->w;
    int text_height = textSurface->h;
    SDL_FreeSurface(textSurface);
    SDL_Rect renderQuad = {.x = (int) x, .y = (int) y, .w = text_width, .h = text_height};
    SDL_RenderCopy(renderer, text, NULL, &renderQuad);
    SDL_DestroyTexture(text);
}

void text(SDL_Renderer* renderer, std::string text_to_display, float x, float y)
{
    text(renderer, text_to_display, x, y, 24);
}