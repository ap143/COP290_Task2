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
    int n = 20;
    bool ***maze;
    SDL_Texture *wall;
    SDL_Texture *grass;
    float ox;
    float oy;
    float grid_length;
    float cell_size;
    Maze();
    Maze(int n, int w, int h);
    void generate();
    void show(SDL_Renderer *renderer, SDL_Window *window);

private:
    void dfs(std::vector<std::vector<bool>> &cells, int i, int j);
};

#endif