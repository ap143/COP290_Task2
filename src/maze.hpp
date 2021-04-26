#ifndef MAZE
#define MAZE

#include "SDL.h"
#include <iostream>
#include "./utils/draw.hpp"
#include <vector>
#include <algorithm>

class Maze
{
public:
    bool ***maze;
    SDL_Texture *wall;
    SDL_Texture *grass;
    Maze();
    Maze(int n);
    void generate();
    void show(SDL_Renderer *renderer, SDL_Window *window);

private:
    int n = 25;
    void dfs(std::vector<std::vector<bool>> &cells, int i, int j);
};

#endif