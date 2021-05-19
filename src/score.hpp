#ifndef SCORE
#define SCORE

#include "maze.hpp"

class Score
{
private:
    // Dsiplay fonts styles
    TTF_Font *font1 = nullptr;
    TTF_Font *font2 = nullptr;
    int font_size = 30 * scale;

    // boolean variable to assert if hovered on Play Again button
    bool hovered = false;

    // Game Renderer
    SDL_Renderer *renderer = nullptr;
    // Game Maze
    Maze *game_maze = nullptr;

    //  Timer
    int countdown;
    int mil_sec;

    SDL_FRect timer_board;

    SDL_Texture *timer = nullptr, *timer_text = nullptr;

    // Score
    SDL_FRect score_board;

    SDL_Texture *score = nullptr, *score_text = nullptr;

    // Text Box details
    SDL_FRect text_box;
    SDL_Rect length_displayed;
    SDL_Texture *text_to_display = nullptr;

    // Play Again
    SDL_FRect play_again_box;
    SDL_Texture *play_again_text = nullptr;

    SDL_Texture *winner = nullptr;
    SDL_Texture *loser = nullptr;
    SDL_Texture *draw = nullptr;

public:
    // Message on Text Box
    std::string message;

    // Kings Deployed
    bool kingDeployed = false;
    // Kings deploy time
    int king_deploy_time;

    // Winner of game -> true if my team wins
    bool win = false;
    bool match_draw = false;

    // Play Again Request sent
    bool play_again = false;
    // Play Again Request received
    bool play_again_request = false;

    Score(SDL_Renderer *renderer, Maze *maze);
    ~Score();

    // Renderer function for timer, score, textbox and other related details
    void show();

    // Updates time left score and message 
    void update();

    // Handle events for play again when game_over
    void handleEvent(SDL_Event event);
};

#endif