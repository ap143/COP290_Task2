#ifndef CHARACTER
#define CHARACTER

#include "maze.hpp"
#include <map>

// Character properties
struct properties
{
    int power;
    int health;
    int speed;

    properties()
    {
    }

    properties(int p, int h, int s)
    {
        power = p;
        health = h;
        speed = s;
    }
};

class Character
{
public:

    int team;
    int level; // level 0/1/2/3
    bool isMyTeam; // 1 -> my team
    int health;
    int sprite_change = 0;

    int animationDirection = -1;

    int attackDir = -1;

    static const int max_speed = 10;

    properties prop;

    SDL_Texture *spriteSheet = nullptr;
    SDL_Texture *attackSheet = nullptr;
    SDL_Renderer *renderer;

    Maze* game_maze;

    float dx = 0;
    float dy = 0;

    // {row, column}
    int currPos[2];
    
    // {x, y}
    int vel[2];

    int currDir = -1;

    bool ready = false;
    bool active = false;
    bool dead = false;

    static SDL_Rect spriteDim[4][3];

    static std::vector<properties> character_data;
    static int total_teams;

    Character();

    Character(SDL_Renderer* renderer, Maze* maze, int teamNum, int level, bool team);
    ~Character();

    void show();
    void deploy(int i, int j);
    void update();
    void setVel(int dir);
    void turn(int dir);
    void attack(int pow);

private:
    float velConst = 0.005;
    int currSprite = 0;

    int currBlast = 0;
    int w, h;

    bool changed = false;

    SDL_Rect blastRect;
    SDL_Rect spriteRect;
    SDL_FRect posRect;

};

#endif