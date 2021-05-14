#ifndef SCORE
#define SCORE

#include "maze.hpp"

class Score
{
private:

    TTF_Font *font;
    int font_size = 12 * scale;

    SDL_Texture *timer, *scoreTexture;

    int countdown;

    SDL_Renderer *renderer;
    Maze *game_maze;

public:
    
    Score(SDL_Renderer *renderer, Maze *maze);
    ~Score();

    void show();
    void update();
    void handleEvent(SDL_Event event);

};

#endif