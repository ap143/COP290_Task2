#ifndef TEAM_VIEW
#define TEAM_VIEW

#include "character.hpp"
#include "network.hpp"
#include "maze.hpp"

extern int gui_width;
extern int gui_height;

class Teamview
{
public:

    std::vector<std::vector<Character*>> characters;
    Teamview();
    Teamview(SDL_Renderer* renderer, Maze* maze, int teamNum, bool self);

    void show();

    static const int lol = 69;

private:

    SDL_Renderer* renderer;

    Maze *game_maze; 

    int team;
    bool isMyTeam;

    const float block_ox = game_maze->ox + game_maze->grid_length;
    const float block_oy = gui_height / 2;
    const float block_width = gui_width - block_ox;
    const float block_height = gui_height - block_oy;

    SDL_Rect src = {.x = 48, .y = 0, .w = 48, .h = 49};

    float tile_height = block_height / 4;
    float bar_height = tile_height / 4;
    float bar_width = (block_width - tile_height) * 0.9;
    float bar_fill = 0;


    SDL_FRect tiles[4] = 
                        {{.x = block_ox, .y = block_oy, .w = tile_height, .h = tile_height},
                        {.x = block_ox, .y = block_oy + tile_height, .w = tile_height, .h = tile_height},
                        {.x = block_ox, .y = block_oy + 2 * tile_height, .w = tile_height, .h = tile_height},
                        {.x = block_ox, .y = block_oy + 3 * tile_height, .w = tile_height, .h = tile_height}
                        };

    int max[4] = {6, 12, 8, 6};
    int color_comb[4][3] = {{0, 0, 255}, {255, 0, 0}, {255, 242, 0}, {149, 0, 255}};

};

#endif