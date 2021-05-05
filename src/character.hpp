#ifndef CHARACTER
#define CHARACTER

#include "SDL.h"
#include <iostream>
#include "./utils/draw.hpp"
#include <vector>
#include <map>

// Character properties
struct properties
{
    int power;
    int health;
    int range;
    int speed;

    properties()
    {
    }

    properties(int p, int h, int r, int s)
    {
        power = p;
        health = h;
        range = r;
        speed = s;
    }
};

class Character
{
public:

    int team;
    int level; // level 0/1/2/3
    bool isMyTeam; // 1 -> my team

    properties prop;

    SDL_Texture *spriteSheet;
    SDL_Renderer *renderer;

    float dx = 0;
    float dy = 0;

    int currPos[2];
    int vel[2];

    Character();

    Character(SDL_Renderer* renderer, int teamNum, int level, bool team);
    ~Character();

    void show();

private:
    float velConst = 0.01;
    int currSprite = 0;

    SDL_Rect spriteRect;
    SDL_FRect posRect;

};

#endif