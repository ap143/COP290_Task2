#ifndef SCORE
#define SCORE

#include "maze.hpp"

class Score
{
private:

    TTF_Font *font1 = nullptr;
    TTF_Font *font2 = nullptr;
    int font_size = 30 * scale;

    SDL_Texture *timer = nullptr, *score = nullptr;
    SDL_Texture *timer_text = nullptr, *score_text = nullptr;

    int king_deploy_time;
    int countdown;
    int mil_sec;

    bool hovered = false;

    SDL_Renderer *renderer = nullptr;
    Maze *game_maze = nullptr;

    SDL_FRect timer_board;
    SDL_FRect score_board;

    // Text Box 
    SDL_FRect text_box;
    SDL_Rect length_displayed;
    SDL_Texture* text_to_display = nullptr;
    int change = 1;
    int width = 0, height = 0;


    // Play Again
    SDL_FRect play_again_box; 

    SDL_Texture* win = nullptr;
    SDL_Texture* lose = nullptr;

    SDL_Texture* play_again_text = nullptr;
public:
    // text changed
    std::string message;

    // Kings Deployed
    bool kingDeployed = false;

    bool play_again = false;
    bool play_again_request = false;

    Score(SDL_Renderer *renderer, Maze *maze);
    ~Score();

    void show();
    void update();
    void handleEvent(SDL_Event event);

};

#endif