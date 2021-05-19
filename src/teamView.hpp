#ifndef TEAM_VIEW
#define TEAM_VIEW

#include "character.hpp"
#include <queue>

// Struct for the use in Dijkstra
struct Point
{
    // Index i and j in the 2D array, i -> row and j -> column
    int i, j;

    // Dist of the point from the source
    long long int dist;

    // Variables to store if their is an enemy on the maze and to check if the point is visited
    bool enemy, visited;

    // Previous point in the path from source
    Point *pre;

    // Enemy character on the point in the maze
    Character *e;

    // Level and count of the enemy character
    int lvl, cnt;

    // Constructor
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

// Compare operator for comparing two points in Dijkstra
struct Compare
{
    bool operator()(Point *const &p1, Point *const &p2)
    {
        return p1->dist > p2->dist;
    }
};

// Class that contains everything
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

    // To render characters and selection zone
    void show();

    // Update function
    void update();

    // To handle event
    void handleEvent(SDL_Event event);
    
    // To deploy a player on the maze
    void deploy(int level, int count, int i, int j);
    
    // To attack a wall on the map
    void attackWall(int i, int j, int dir, int power);

private:
    // Renderer
    SDL_Renderer *renderer;

    // Game maze
    Maze *game_maze;

    // Team number
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