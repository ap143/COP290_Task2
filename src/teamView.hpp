#ifndef TEAM_VIEW
#define TEAM_VIEW

#include "character.hpp"
#include <queue>

struct Point
{
    int i, j;
    long long int dist;
    bool enemy, visited;

    Point *pre;
    Character *e;
    int lvl, cnt;

    Point(int i, int j, int dist, bool enemy, bool visited)
    {
        this->i = i;
        this->j = j;
        this->dist = dist;
        this->enemy = enemy;
        this->visited = visited;
        this->pre = nullptr;
        this->e = nullptr;
        lvl = -1;
        cnt = -1;
    }
};

struct Compare
{
    bool operator()(Point* const& p1, Point* const& p2)
    {
        return p1->dist > p2->dist;
    }
};

class Teamview
{
public:

    std::vector<std::vector<Character*>> characters;

    int count[4] = {1, 4, 2, 1};

    int activeLevel = -1;

    bool kingDeployed = false;
    bool opponentKingDeployed = false;

    int deployRange;
    bool deployingNow = false;

    Teamview *enemyTeam;
    
    Teamview(SDL_Renderer* renderer, Maze* maze, int teamNum, bool self);

    void show();
    void update();
    void handleEvent(SDL_Event event);
    void deploy(int level, int count, int i, int j); 
    void attackWall(int i, int j, int dir, int power);

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

    void setNextDest(Character* c, int level, int cnt);
    void kingAttack(Character *c);
};

#endif