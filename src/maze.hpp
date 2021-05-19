#ifndef MAZE
#define MAZE

#include "./utils/draw.hpp"
#include <vector>
#include <algorithm>
#include <set>

class Maze
{
public:
    // Number of cells in each row and column
    int n = 20;

    // Boolean 3D maze of size n * n * 4
    // 0, 1, 2, 3 -> top wall, right wall, bottom wall, left wall of each cell
    // false if wall is there walse true
    bool ***maze;

    // Wall texture
    SDL_Texture *wall = nullptr;
    // Grass texture
    SDL_Texture *grass = nullptr;

    // Origin of maze
    float ox;
    float oy;

    // Length of maze
    float grid_length;

    // Length of each cell
    float cell_size;

    Maze(int n, int w, int h);
    ~Maze();

    // Generates a random maze for game play
    void generate();

    // renderer function to display maze on screen
    void show(SDL_Renderer *renderer, SDL_Window *window);

    // Health of each wall in maze
    std::vector<int> maze_health;
    const int wall_health = 5;

private:
    // Max Wall Health
    int max_health;

    // Helper funtion to generate maze
    // Maze generated using depth first search scheme
    void dfs(std::vector<std::vector<bool>> &cells, int i, int j);
};

#endif