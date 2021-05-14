#include "score.hpp"

Score::Score(SDL_Renderer *renderer, Maze *maze)
{
    font = TTF_OpenFont("./assets/fonts/CONSOLAB.TTF", font_size);

    this->renderer = renderer;
    this->game_maze = maze;

    countdown = 120;    

    color(renderer, 255, 255);
    timer = text(renderer, std::to_string(countdown) + "s", font);

    color(renderer, 0, 255, 0, 255);
    scoreTexture = text(renderer, std::to_string(myTeamScore) + " - " + std::to_string(opponentTeamScore), font);

}

Score::~Score()
{
    TTF_CloseFont(font);
    SDL_DestroyTexture(scoreTexture);
    SDL_DestroyTexture(timer);
}

void handleEvent(SDL_Event event)
{
    
}


void update()
{

}

void show()
{

}