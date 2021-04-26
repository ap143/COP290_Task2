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
    int level; // level 0/1/2/3
    bool team;
    properties prop;

    SDL_Texture *spriteSheet;

    float dx = 0;
    float dy = 0;

    int currPos[2];
    int vel[2];

    Character(int level, bool team);
    ~Character();

    void updatePos();
    /*
     1. Calculate new position using djkstra
     2. update the prevPos and currPos
     3. Called in for every frame in Game->update
    */

    void move(SDL_Renderer *ren);
    /*
     1. animate sprite according to prevPos and currPos
     2. Called in Game->render.
    */

    void attack();
    /*
     1. Check opponent in current cell and attack.
    */

    void fight();
    /*
     1. Sprite animation of fight scene
    */

    bool isAlive();
    /*
     1. if health <=0 then destroy character
    */

    static Character born(int level, struct properties p);
    /*
     1. When mouse event of creating a new character then this is called.
     2. calls contructor.
     3. marks the cell in which it is intiated.
    */

    void show();

private:
    float velConst = 0.1;
};

#endif