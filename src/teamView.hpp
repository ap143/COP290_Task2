#ifndef TEAM_VIEW
#define TEAM_VIEW

#include "maze.hpp"

extern int gui_width;
extern int gui_height;

class Teamview
{
public:

    std::vector<std::vector<Character*>> characters;
    
    Teamview(SDL_Renderer* renderer, Maze* maze, int teamNum, bool self);

    void show();

    static const int lol = 69;

private:

    SDL_Renderer* renderer;

    Maze *game_maze; 

    int team;
    bool isMyTeam;

    float block_ox;
    float block_oy;
    float block_width;
    float block_height;

    SDL_Rect src;

    float tile_height;
    float bar_height;
    float bar_width;
    float bar_fill;


    SDL_FRect tiles[4];

    int max[4] = {6, 12, 8, 6};
    int color_comb[4][3] = {{0, 0, 255}, {255, 0, 0}, {255, 242, 0}, {149, 0, 255}};

};

#endif