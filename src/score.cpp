#include "score.hpp"

Score::Score(SDL_Renderer *renderer, Maze *maze)
{
    // Openeing required fonts
    font1 = TTF_OpenFont("./assets/fonts/CONSOLAB.TTF", font_size);
    font2 = TTF_OpenFont("./assets/fonts/CONSOLAB.TTF", font_size / 2);

    this->renderer = renderer;
    this->game_maze = maze;

    // Initializing timers
    king_deploy_time = 10;
    countdown = 40;
    mil_sec = 60;

    // Setting up positions for timer, score, text boxes and play again button

    timer_board = {.x = game_maze->ox + game_maze->grid_length * (float)1.01, .y = (float)gui_height / 8, .w = gui_width - game_maze->ox - game_maze->grid_length, .h = (float)gui_height / 12};

    score_board = {.x = game_maze->ox + game_maze->grid_length * (float)1.01, .y = timer_board.h * 3 / 2 + timer_board.y, .w = gui_width - game_maze->ox - game_maze->grid_length, .h = (float)gui_height / 12};

    text_box = {.x = game_maze->ox + game_maze->grid_length * (float)1.01, .y = timer_board.y + 5 * timer_board.h / 2, .w = gui_width - game_maze->ox - game_maze->grid_length, .h = (float)gui_height / 6};

    play_again_box = {.x = game_maze->ox + game_maze->grid_length * (float)1.01 + (gui_width - game_maze->ox - game_maze->grid_length) / 6, .y = text_box.y + text_box.h * (float)1.05, .w = (gui_width - game_maze->ox - game_maze->grid_length) * 2 / 3, .h = (float)gui_height / 12};

    // Initial loading of  all textures for texts 

    color(renderer, 255, 255, 255, 255);
    timer = text(renderer, std::to_string(king_deploy_time), font1);
    timer_text = text(renderer, "Time Left", font1);

    color(renderer, 0, 255, 0, 255);
    score = text(renderer, std::to_string(myTeamScore) + " - " + std::to_string(opponentTeamScore), font1);
    score_text = text(renderer, "Score", font1);

    message = "Deploy King!";
    color(renderer, 255, 255, 255, 255);
    text_to_display = text(renderer, message, font1);

    color(renderer, 0, 0, 0, 255);
    play_again_text = text(renderer, "Play Again", font1);

    winner = loadTexture("./assets/images/Winner.png", renderer);
    loser = loadTexture("./assets/images/Loser.png", renderer);
    draw = loadTexture("./assets/images/Draw.png", renderer);
}

Score::~Score()
{
    TTF_CloseFont(font1);
    TTF_CloseFont(font2);

    if (score != NULL)
    {
        SDL_DestroyTexture(score);
    }
    if (score_text != NULL)
    {
        SDL_DestroyTexture(score_text);
    }
    if (timer != NULL)
    {
        SDL_DestroyTexture(timer);
    }
    if (timer_text != NULL)
    {
        SDL_DestroyTexture(timer_text);
    }
    if (text_to_display != NULL)
    {
        SDL_DestroyTexture(text_to_display);
    }
    if (play_again_text != NULL)
    {
        SDL_DestroyTexture(play_again_text);
    }
}

void Score::handleEvent(SDL_Event event)
{
    // If game over then handling events of play again button

    if (game_over)
    {
        float x = event.button.x;
        float y = event.button.y;

        if (event.type == SDL_MOUSEBUTTONUP)
        {

            if (inRect(x, y, play_again_box.x, play_again_box.y, play_again_box.w, play_again_box.h))
            {
                play_again = true;
                sendMessage(PLAY_AGAIN + std::string("1"));
                return;
            }
        }

        if (event.type == SDL_MOUSEMOTION)
        {
            hovered = inRect(x, y, play_again_box.x, play_again_box.y, play_again_box.w, play_again_box.h);
        }
    }
}

void Score::update()
{
    // Updating countdown
    if (!game_over)
    {
        // If both Kings not deployed then king deploy timer else game begins then normal game countdown begins
        // If count down equals zero then game overs
        if (!kingDeployed)
        {
            if (king_deploy_time != 0)
            {
                mil_sec--;
                if (mil_sec == 0)
                {
                    king_deploy_time--;
                    if (timer != NULL)
                    {
                        SDL_DestroyTexture(timer);
                    }
                    color(renderer, 255, 0, 0, 0);
                    timer = text(renderer, std::to_string(king_deploy_time), font1);

                    mil_sec = 60;
                }
            }
        }
        else if (countdown != 0)
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
                timer = text(renderer, std::to_string(countdown), font1);

                mil_sec = 60;
            }
            if (message != "")
            {
                color(renderer, 255, 255, 255, 255);
                if (text_to_display != NULL)
                {
                    SDL_DestroyTexture(text_to_display);
                }
                message = "";
                text_to_display = text(renderer, message, font1);
            }
        }
        else if (countdown == 0)
        {
            color(renderer, 255, 0, 0, 255);
            if (text_to_display != NULL)
            {
                SDL_DestroyTexture(text_to_display);
            }
            message = "Game Over";
            text_to_display = text(renderer, "Game Over", font1);
            game_over = true;

            if (myTeamScore > opponentTeamScore)
            {
                match_result = 0;
            }
            else if (myTeamScore == opponentTeamScore)
            {
                match_result = 1;
            }
            else
            {
                match_result = 2;
            }
        }
        else
        {

        }

        // Updating score

        if (score != NULL)
        {
            SDL_DestroyTexture(score);
        }
        color(renderer, 0, 255, 0, 255);
        score = text(renderer, std::to_string(myTeamScore) + " - " + std::to_string(opponentTeamScore), font1);
    }

    // Texts Update

    if (play_again_request && play_again)
    {
        if (message != "Lets Play Again!")
        {
            color(renderer, 255, 255);
            if (text_to_display != NULL)
            {
                SDL_DestroyTexture(text_to_display);
            }
            message = "Lets Play Again!";
            text_to_display = text(renderer, message, font1);
        }
    }
    else if (play_again_request)
    {
        if (message != opponent_name + " Wants to Play Again..")
        {
            color(renderer, 255, 255);
            if (text_to_display != NULL)
            {
                SDL_DestroyTexture(text_to_display);
            }
            message = opponent_name + " Wants to Play Again..";
            text_to_display = text(renderer, message, font2);
        }
    }
    else if (play_again)
    {
        if (message != "Waiting for " + opponent_name)
        {
            color(renderer, 255, 255);
            if (text_to_display != NULL)
            {
                SDL_DestroyTexture(text_to_display);
            }
            message = "Waiting for " + opponent_name;
            text_to_display = text(renderer, message, font2);
        }
    }
    else if (game_over)
    {
        if (message != "Game Over")
        {
            color(renderer, 255, 0, 0, 255);
            if (text_to_display != NULL)
            {
                SDL_DestroyTexture(text_to_display);
            }
            message = "Game Over";
            text_to_display = text(renderer, message, font1);
        }
    }
}

void Score::show()
{
    // timer
    color(renderer, 217, 179, 11, 255);
    rectCenter(renderer, timer_board.x + timer_board.w / 2, timer_board.y - timer_board.h / 2 + 3 * timer_board.h / 2, timer_board.w * (float)0.9 * 1.05, (float)1.05 * 3 * timer_board.h, 1, true);

    color(renderer, 0, 255);

    rectCenter(renderer, timer_board.x + timer_board.w / 2, timer_board.y - timer_board.h / 4, timer_board.w * (float)0.9, timer_board.h / 2, 1, true);
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
    imageCenter(renderer, text_to_display, text_box.x + text_box.w / 2, text_box.y + text_box.h / 2);

    // game over

    if (game_over)
    {
        // Play Again Button
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

        // Game Result
        if (match_result == 0)
        {
            imageCenter(renderer, winner, game_maze->ox + game_maze->grid_length / 2, game_maze->oy + game_maze->grid_length / 2, scale * 2); 
        }
        else if (match_result == 1)
        {
            imageCenter(renderer, draw, game_maze->ox + game_maze->grid_length / 2, game_maze->oy + game_maze->grid_length / 2, scale * 2);
        }
        else if (match_result == 2)
        {
            imageCenter(renderer, loser, game_maze->ox + game_maze->grid_length / 2, game_maze->oy + game_maze->grid_length / 2, scale * 2);
        }
    }
}