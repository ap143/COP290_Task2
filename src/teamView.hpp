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
    bool operator()(Point *const &p1, Point *const &p2)
    {
        return p1->dist > p2->dist;
    }
};

class Teamview
{
public:
    // Characters of my Team
    std::vector<std::vector<Character *>> characters;

    // Number of plyers of each level
    int count[4] = {1, 4, 2, 1};

    // Active Level in player deploying
    int activeLevel = -1;

    // Assertes when king of my team and opponent team deployed
    bool kingDeployed = false;
    bool opponentKingDeployed = false;

    // Deplroy Range of King
    int deployRange;

    // When Active level != -1 and a cell is to be selected in the maze to deplroy character
    bool deployingNow = false;

    // Textures for number of player remaing of each level in my team
    SDL_Texture *count_text[4] = {nullptr, nullptr, nullptr, nullptr};
    int count_text_size = 20 * scale;

    // Enemy team details
    Teamview *enemyTeam;

    Teamview(SDL_Renderer *renderer, Maze *maze, int teamNum, bool self);
    ~Teamview();

    void show();
    void update();
    void handleEvent(SDL_Event event);
    void deploy(int level, int count, int i, int j);
    void attackWall(int i, int j, int dir, int power);

private:
    SDL_Renderer *renderer;

    Maze *game_maze;

    // team number
    int team;
    // Asserted when this is my team
    bool isMyTeam;

    // Team Details display dimensions
    float block_ox;
    float block_oy;
    float block_width;
    float block_height;

    float tile_height;
    float bar_height;
    float bar_width;
    float bar_fill;

    SDL_Rect src;
    SDL_FRect tiles[4];
    SDL_Texture *tile_tex = nullptr;

    // For Setting next Destination of a player using shortest path algorithm
    void setNextDest(Character *c, int level, int cnt);

    // When king attacks
    void kingAttack(Character *c);
};

#endif