#ifndef TEAM_VIEW
#define TEAM_VIEW

#include "character.hpp"

extern int gui_width;
extern int gui_height;

extern float scale;

class Teamview
{
public:

    std::vector<std::vector<Character*>> characters;

    int count[4] = {1, 4, 2, 1};

    int activeLevel = -1;

    bool kingDeployed = false;
    bool opponentKingDeployed = false;

    int deployRange;
    
    Teamview(SDL_Renderer* renderer, Maze* maze, int teamNum, bool self);

    void show();

    void update();
    
    void handleEvent(SDL_Event event);

    void deploy(int level, int count, int i, int j); 

    void setNextDest(Character* c); 

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

    SDL_Texture* count_text[4] = {nullptr, nullptr, nullptr, nullptr};
    int count_text_size = 10 * scale;
};

#endif