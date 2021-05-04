#include "draw.hpp"

// Variables to store current color of the renderer
// Change these values in setColor()
Uint8 _r = 0;
Uint8 _g = 0;
Uint8 _b = 0;
Uint8 _a = 255;

void color(SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    _r = r;
    _g = g;
    _b = b;
    _a = a;
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void color(SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b)
{
    color(renderer, r, g, b, 255);
}

void color(SDL_Renderer *renderer, Uint8 grey, Uint8 alpha)
{
    color(renderer, grey, grey, grey, alpha);
}

void color(SDL_Renderer *renderer, Uint8 grey)
{
    color(renderer, grey, grey, grey, 255);
}

void line(SDL_Renderer *renderer, float x1, float y1, float x2, float y2)
{
    SDL_RenderDrawLineF(renderer, x1, y1, x2, y2);
}

void point(SDL_Renderer *renderer, float x, float y)
{
    SDL_RenderDrawPointF(renderer, x, y);
}

void rect(SDL_Renderer *renderer, float x, float y, float width, float height, bool filled)
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

void rectCenter(SDL_Renderer *renderer, float cx, float cy, float width, float height, float scale, bool filled)
{
    SDL_FRect rect = getSDLFRect(cx - width / 2 * scale, cy - height / 2 * scale, width * scale, height * scale);
    if (filled)
    {
        SDL_RenderFillRectF(renderer, &rect);
    }
    else
    {
        SDL_RenderDrawRectF(renderer, &rect);
    }
}

SDL_Texture *text(SDL_Renderer *renderer, std::string text_to_display, float size)
{
    TTF_Font *font = TTF_OpenFont("./assets/fonts/CONSOLAB.TTF", size);
    SDL_Color fontColor = {.r = _r, .g = _g, .b = _b, .a = _a};
    SDL_Surface *textSurface = TTF_RenderText_Blended(font, text_to_display.c_str(), fontColor);
    SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    return text;
}

SDL_Texture *text(SDL_Renderer *renderer, std::string text_to_display)
{
    return text(renderer, text_to_display, 24);
}

SDL_Texture *loadTexture(const char *texture, SDL_Renderer *ren)
{
    SDL_Surface *tempSurface = IMG_Load(texture);
    if (tempSurface == NULL)
        std::cout << "surface loading failed" << std::endl;
    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, tempSurface);
    SDL_FreeSurface(tempSurface);
    return tex;
}

void imageCenter(SDL_Renderer *renderer, SDL_Texture *texture, float x, float y)
{
    int width, height;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    SDL_FRect rect = getSDLFRect(x - width / 2, y - height / 2, width, height);
    SDL_RenderCopyF(renderer, texture, NULL, &rect);
}

void imageCenter(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *src, float x, float y, float width, float height)
{
    SDL_FRect rect = getSDLFRect(x - width / 2, y - height / 2, width, height);
    SDL_RenderCopyF(renderer, texture, src, &rect);
}

void imageCenter(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *src, float x, float y, float width, float height, float scale)
{
    SDL_FRect rect = getSDLFRect(x - width * scale / 2, y - height * scale / 2, width * scale, height * scale);
    SDL_RenderCopyF(renderer, texture, src, &rect);
}

void image(SDL_Renderer *renderer, SDL_Texture *texture, float sx, float sy, float sw, float sh, float dx,
           float dy, float dw, float dh, float angle, SDL_RendererFlip flip)
{
    SDL_Rect src = getSDLRect(sx, sy, sw, sh);
    SDL_FRect dst = getSDLFRect(dx, dy, dw, dh);

    SDL_RenderCopyExF(renderer, texture, &src, &dst, angle, NULL, flip);
}

void image(SDL_Renderer *renderer, SDL_Texture *texture, float dx,
           float dy, float dw, float dh, float angle, SDL_RendererFlip flip)
{
    SDL_FRect dst = getSDLFRect(dx, dy, dw, dh);

    SDL_RenderCopyExF(renderer, texture, NULL, &dst, angle, NULL, flip);
}

void image(SDL_Renderer *renderer, SDL_Texture *texture, float dx,
           float dy, float angle, SDL_RendererFlip flip)
{
    int width, height;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    SDL_FRect rect = getSDLFRect(dx, dy, width, height);
    SDL_RenderCopyExF(renderer, texture, NULL, &rect, angle, NULL, flip);
}

void image(SDL_Renderer *renderer, SDL_Texture *texture, float angle, SDL_RendererFlip flip)
{
    SDL_RenderCopyEx(renderer, texture, NULL, NULL, angle, NULL, flip);
}

void backgroundImage(SDL_Renderer *renderer, SDL_Texture *texture)
{
    SDL_RenderCopy(renderer, texture, NULL, NULL);
}

SDL_FRect getSDLFRect(float x, float y, float width, float height)
{
    SDL_FRect rect = {.x = x, .y = y, .w = width, .h = height};
    return rect;
}

SDL_Rect getSDLRect(float x, float y, float width, float height)
{
    SDL_Rect rect = {.x = (int)x, .y = (int)y, .w = (int)width, .h = (int)height};
    return rect;
}
