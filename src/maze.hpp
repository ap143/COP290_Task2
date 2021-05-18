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

    SDL_Texture *wall = nullptr;
    SDL_Texture *grass = nullptr;

    float ox;
    float oy;
    float grid_length;
    float cell_size;

    Maze(int n, int w, int h);
    ~Maze();

    void generate();
    void show(SDL_Renderer *renderer, SDL_Window *window);

    std::vector<int> maze_health;
    const int wall_health = 5;
    int max_health;

private:
    void dfs(std::vector<std::vector<bool>> &cells, int i, int j);
};

#endif