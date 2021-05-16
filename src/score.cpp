#include "score.hpp"

Score::Score(SDL_Renderer *renderer, Maze *maze)
{
    font = TTF_OpenFont("./assets/fonts/CONSOLAB.TTF", font_size);

    this->renderer = renderer;
    this->game_maze = maze;

    countdown = 120; 
    mil_sec = 60;   

    timer_board = {.x = game_maze->ox + game_maze->grid_length * (float)1.01, .y = (float)gui_height / 8, .w = gui_width - game_maze->ox - game_maze->grid_length, .h = (float)gui_height / 12 };

    score_board = {.x = game_maze->ox + game_maze->grid_length * (float)1.01, .y = timer_board.h * 3 / 2 + timer_board.y, .w = gui_width - game_maze->ox - game_maze->grid_length, .h = (float)gui_height / 12 };

    text_box = {.x = game_maze->ox + game_maze->grid_length * (float)1.01, .y = timer_board.y + 5 * timer_board.h / 2, .w = gui_width - game_maze->ox - game_maze->grid_length, .h = (float)gui_height / 6};

    play_again_box = {.x = game_maze->ox + game_maze->grid_length * (float)1.01 + (gui_width - game_maze->ox - game_maze->grid_length) / 6, .y = text_box.y + text_box.h * (float)1.05, .w = (gui_width - game_maze->ox - game_maze->grid_length) * 2 / 3, .h = (float)gui_height / 12};

    length_displayed = {.x = 0, .y = 0};

    color(renderer, 255, 255, 255, 255);
    timer = text(renderer, std::to_string(countdown), font);
    timer_text = text(renderer, "Time Left", font);

    color(renderer, 0, 255, 0, 255);
    score = text(renderer, std::to_string(myTeamScore) + " - " + std::to_string(opponentTeamScore), font);
    score_text = text(renderer, "Score", font);

    color(renderer, 255, 255, 255, 255);
    text_to_display = text(renderer, "Deploy King!", font);

    color(renderer, 0, 0, 0, 255);
    play_again_text = text(renderer, "Play Again", font);
}

Score::~Score()
{
    TTF_CloseFont(font);
    SDL_DestroyTexture(score);
    SDL_DestroyTexture(timer);
}

void Score::handleEvent(SDL_Event event)
{
    if(game_over)
    {

        float x = event.button.x;
        float y = event.button.y;

        if (event.type == SDL_MOUSEBUTTONUP)
        {
            
            if (inRect(x, y, play_again_box.x , play_again_box.y, play_again_box.w, play_again_box.h))
            {
                play_again = true;
                changed = true;
                sendMessage(PLAY_AGAIN + std::string("1"));   
                return;
            }
        }

        if (event.type == SDL_MOUSEMOTION)
        {
            hovered = inRect(x, y, play_again_box.x , play_again_box.y, play_again_box.w, play_again_box.h);
        }
    }   
}


void Score::update()
{
    // countdown 
    if (!game_over)
    {
        if (countdown != 0)
        {
            mil_sec--;
            if (mil_sec == 0)
            {
                countdown--;

                if (timer != NULL)
                {
                    SDL_DestroyTexture(timer);
                }
                if (countdown <= 10)
                {
                    color(renderer, 255, 0, 0, 0);
                }
                else
                {
                    color(renderer, 255, 255, 255, 255);
                }
                timer = text(renderer, std::to_string(countdown), font);

                mil_sec = 60;
            } 
        }
        else
        {
            color(renderer, 255, 0, 0, 255);
            if (text_to_display != NULL)
            {
                SDL_DestroyTexture(text_to_display);
            }
            text_to_display = text(renderer, "Game Over", font);
        }

        // score
        if (score != NULL)
        {
            SDL_DestroyTexture(score);
        }
        color(renderer, 0, 255, 0, 255);
        score = text(renderer, std::to_string(myTeamScore) + " - " + std::to_string(opponentTeamScore), font);
    }
    if (changed)
    {   
        if (play_again_request && play_again)
        {
            color(renderer, 255, 255);
            if (text_to_display != NULL)
            {
                SDL_DestroyTexture(text_to_display);
            }
            text_to_display = text(renderer, "Lets Play Again!", font); 
        }
        else if (play_again_request)
        {
            color(renderer, 255, 255);
            if (text_to_display != NULL)
            {
                SDL_DestroyTexture(text_to_display);
            }
            text_to_display = text(renderer, opponent_name + " Wants to Play Again..", font);   
        } 
        else if (play_again)
        {
            color(renderer, 255, 255);
            if (text_to_display != NULL)
            {
                SDL_DestroyTexture(text_to_display);
            }
            text_to_display = text(renderer, "Waiting for " + opponent_name, font);       
        } 
        else
        {
            color(renderer, 255, 0, 0, 255);
            if (text_to_display != NULL)
            {
                SDL_DestroyTexture(text_to_display);
            }
            text_to_display = text(renderer, "Game Over", font);   
        }

        changed = false;
    }
    if (text_to_display != NULL)
    {
        SDL_QueryTexture(text_to_display, NULL, NULL, &width, &height);  
        length_displayed.x = (width + length_displayed.x + change) % width;
    }

}

void Score::show()
{
    // timer
    color(renderer, 217, 179, 11, 255);
    rectCenter(renderer, timer_board. x + timer_board.w / 2, timer_board.y - timer_board.h / 2 + 3 * timer_board.h / 2, timer_board.w * (float)0.9 * 1.05, (float)1.05 * 3 * timer_board.h, 1, true);

    color(renderer, 0, 255);

    rectCenter(renderer, timer_board. x + timer_board.w / 2, timer_board.y - timer_board.h / 4, timer_board.w * (float)0.9, timer_board.h / 2, 1, true);
    imageCenter(renderer, timer_text, timer_board.x + timer_board.w / 2, timer_board.y - timer_board.h / 4);

    rectCenter(renderer, timer_board.x + timer_board.w / 2, timer_board.y + timer_board.h / 2, timer_board.w * (float)0.9, timer_board.h, 1, true);
    imageCenter(renderer, timer, timer_board.x + timer_board.w / 2, timer_board.y + timer_board.h / 2);

    // score
    color(renderer, 0, 255);

    rectCenter(renderer, score_board.x + score_board.w / 2, score_board.y - score_board.h / 4, score_board.w * (float)0.9, score_board.h / 2, 1, true);
    imageCenter(renderer, score_text, score_board.x + score_board.w / 2, score_board.y - score_board.h / 4);

    rectCenter(renderer, score_board.x + score_board.w / 2, score_board.y + score_board.h / 2, score_board.w * (float)0.9, score_board.h, 1, true);
    imageCenter(renderer, score, score_board.x + score_board.w / 2, score_board.y + score_board.h / 2);

    // text_box
    color(renderer, 217, 179, 11, 255);
    rectCenter(renderer, text_box.x + text_box.w / 2, text_box.y + text_box.h / 2, text_box.w * (float)0.9 * 1.05, (float)1.05 * text_box.h, 1, true);

    color(renderer, 0, 255);
    rectCenter(renderer, text_box.x + text_box.w / 2, text_box.y + text_box.h / 2, text_box.w * (float)0.9, text_box.h, 1, true);
    image(renderer, text_to_display, length_displayed.x, length_displayed.y , std::min(text_box.w, (float)width - length_displayed.x), float(height), text_box.x + text_box.w * float(0.1), text_box.y + text_box.h / 4,  std::min(text_box.w, (float)width - length_displayed.x), (float)height, 0, SDL_FLIP_NONE);

    if (game_over)
    {
        if (hovered)
        {
            color(renderer, 217, 179, 11, 255);
            rectCenter(renderer, play_again_box.x + play_again_box.w / 2, play_again_box.y + play_again_box.h / 2, play_again_box.w * 0.9, play_again_box.h, 1.05, true);
        }
        else
        {
            color(renderer, 255, 255);
            rectCenter(renderer, play_again_box.x + play_again_box.w / 2, play_again_box.y + play_again_box.h / 2, play_again_box.w * (float)0.9, play_again_box.h, 1, true);
        }
        imageCenter(renderer, play_again_text, play_again_box.x + play_again_box.w / 2, play_again_box.y + play_again_box.h / 2);
    }
}