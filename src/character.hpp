#ifndef CHARACTER
#define CHARACTER

#include "maze.hpp"
#include <map>

// Character properties
struct properties
{
    int power;
    float health;
    int speed;
    float heal;

    properties()
    {
    }

    properties(int p, float h, int s, float hl)
    {
        power = p;
        health = h;
        speed = s;
        heal = hl * health / 10000;
    }
};

class Character
{
public:
    // Team Number
    int team;

    // Level of Character
    int level;

    // Character in my team -> true
    bool isMyTeam;

    static int total_teams;

    // Properties of each character-> power, health, speed, heal
    properties prop;

    // Maze of game
    Maze *game_maze;

    // {row, column}
    int currPos[2];

    // {x, y}
    int vel[2];

    // Direction in which charcater moves currently
    int currDir = -1;

    // Dir of attack
    int attackDir = -1;

    // If character is ready to move in new direction
    bool ready = false;

    // Character is currently active i.e it is deployed in maze
    bool active = false;

    // If character is killed
    bool dead = false;

    // Properties of each character in every team.
    static std::vector<properties> character_data;

    // Sprite Sheet 
    SDL_Texture *spriteSheet = nullptr;

    Character();
    Character(SDL_Renderer *renderer, Maze *maze, int teamNum, int level, bool team);
    ~Character();

    // renderer method for Character
    void show();

    // To deploy character in maze
    void deploy(int i, int j);

    // Updates psotion, vel, direction and properties of character
    void update();

    // set Vel according to current direction
    void setVel(int dir);

    // to change direction 
    void turn(int dir);

    // updates health when attacked
    void attack(int pow);

private:
    // rel Vel
    float velConst = 0.005;

    // Max Speed
    static const int max_speed = 10;

    // Current sprite in sprite sheet in a row
    int currSprite = 0;

    // Current blast texture in attack sheet
    int currBlast = 0;
    // Width and Height of attack sheet
    int w, h;

    // Blast's w and h on screen
    float attackW, attackH;

    // If characters's current cell has changed
    bool changed = false;
    // If sprite changes for character in current move
    int sprite_change = 0;

    // Relative change in position from centre of current cell
    float dx = 0;
    float dy = 0;

    // int animationDirection = -1;

    // Current health of Character
    int health;

    // src rect and dest rect
    SDL_Rect blastRect;
    SDL_Rect spriteRect;
    SDL_FRect posRect;

    // Dimensions of sprites in texture sheet for src rect
    static SDL_Rect spriteDim[4][3];

    // Attack tectures
    SDL_Texture *attackSheet = nullptr;

    SDL_Renderer *renderer;
};

#endif