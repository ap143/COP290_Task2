#ifndef MAZE
#define MAZE

#include "./utils/draw.hpp"
#include <vector>
#include <algorithm>
#include <set>

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

    Maze(int n, int w, int h);

    void generate();
    void show(SDL_Renderer *renderer, SDL_Window *window);

    std::vector<int> maze_health;
    const int wall_health = 1500;

private:
    void dfs(std::vector<std::vector<bool>> &cells, int i, int j);
};

#endif