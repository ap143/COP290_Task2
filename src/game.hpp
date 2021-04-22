#ifndef GAME
#define GAME

#include "SDL.h"
#include <iostream>
#include "./utils/draw.hpp"
#include <vector>
#include <set>


class Maze{
public:
    bool*** maze;
    SDL_Texture* wall;
    SDL_Texture* grass;
    Maze();
    Maze(int n);
    void generate();
    void show(SDL_Renderer* renderer, SDL_Window* window);

private:
    int n = 25;
    void dfs(std::vector<std::vector<bool>> &cells, int i, int j);
};


class Character{
    public:
    int level; // level 0/1/2/3
    int power;
    int health;
    int speed;
    int range;
    SDL_Texture* tex;

    Character();
};

class Game{

public:

    Game();
    ~Game();

    void init(const char* title, int xpos, int ypos, int width, int height);

    void handleEvents();
    void update();
    void render();
    void clean();

    bool running() {return isRunning;};

private:

    bool isRunning;
    Maze* game_maze;
    SDL_Window* window;
    SDL_Renderer* renderer;

};




#endif


/*

Maze(int n);

int n;

bool maze[n][n][4];

void generate()
{
    maze generartion();
}

void show()
{
    // draw maze on screen;
}

prop
{
    health
    speed
    damage
}

map<type, struct prop>

Character(Name, Type)

health

show()
{

}

born(int i, int j)
{

}

kill()
{

}

move()
{
    
}

*/