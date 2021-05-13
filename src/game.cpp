#include "game.hpp"

Game::Game()
{
    game_maze = new Maze(13, gui_width, gui_height);
    state = 0;
}

Game::~Game()
{
}

void Game::init(const char *title, int xpos, int ypos, int width, int height)
{

    isRunning = true;

    window = SDL_CreateWindow(title, xpos, ypos, width, height, 0);

    if (window == NULL)
    {
        std::cerr << "Error creating window..." << std::endl;
        exit(-1);
    }
    else
    {
        std::cout << "Window created..." << std::endl;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    if (renderer == NULL)
    {
        std::cerr << "Error creating renderer..." << std::endl;
        exit(-1);
    }
    else
    {
        std::cout << "Renderer created..." << std::endl;
    }

    SDL_StartTextInput();
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    gui = new Gui(window, renderer);

    play_again_box = {.x = (float)gui_width / 2 - (float) gui_width / 8, .y = (float)gui_height * 4 / 5 - (float) gui_height / 24, .w =(float) gui_width / 4, .h =(float) gui_height / 12};

    loadAllTextures();


    isRunning = true;
}

void Game::handleEvents()
{

    if (game_over)
    {
        state = 99;
    }

    SDL_FlushEvent(SDL_MOUSEMOTION);
    SDL_FlushEvent(SDL_KEYDOWN);

    SDL_Event event;
    SDL_PollEvent(&event);

    if (event.type == SDL_QUIT)
    {
        sendMessage(END_GAME + std::string("0"));
        isRunning = false;
        state = -1;
        return;
    }

    if (state < 4)
    {
        gui->event(event, state);
    }
    else if (state == 7)
    {
        myTeam->handleEvent(event);
    }
    else if (state == 99)
    {
        if (event.type != SDL_MOUSEBUTTONUP)
        {
            return;
        }
        float x = event.button.x;
        float y = event.button.y;
        
        if (inRect(x, y, play_again_box.x , play_again_box.y, play_again_box.w, play_again_box.h))
        {
            play_again = true;
            sendMessage(PLAY_AGAIN + std::string("1"));
        }
    }

}

void Game::update()
{
    if (state == 4)
    {
        if (gui->isHost)
        {
            game_maze->generate();
        }
        curRowToSend = 0;
        state++;
    }
    else if (state == 5 && gui->isHost && curRowToSend < game_maze->n)
    {
        if (opponentTeamNum < 0)
        {
            return;
        }
        std::string rowNum = ((curRowToSend < 10) ? "0" : "") + std::to_string(curRowToSend);
        std::string message = MAZE_STRUCT + rowNum;
        for (int i = 0; i < game_maze->n; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                message += std::to_string((int) game_maze->maze[curRowToSend][i][j]);
            }
        }
        curRowToSend++;
        sendMessage(message);
        if (curRowToSend == game_maze->n)
        {
            sendMessage(READY+std::string("0"));
        }
    }
    else if (state == 5 && opponentReady && curRowToSend == game_maze->n)
    {
        state++;
    }
    else if (state == 6)
    {
        myTeam = new Teamview(renderer, game_maze, teamNum, true);
        opponentTeam = new Teamview(renderer, game_maze, opponentTeamNum, false);
        myTeam->enemyTeam = opponentTeam;
        opponentTeam->enemyTeam = myTeam;
        state++;
    }
    else if (state == 7)
    {
        myTeam->update();
        opponentTeam->update();
    }
    else if (state == 99)
    {
        if (play_again && play_again_request)
        {
            restartGame();
        }
    }
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    if (state < 4)
    {
        gui->show(state);
    }
    else if (state == 4)
    {
        return;
    }
    else if (state == 5)
    {
        drawMazeLoad();
    }
    else if (state == 6)
    {   
        drawMazeLoad();
    }
    else if (state == 7)
    {
        game_maze->show(renderer, window);
        myTeam->show();
        opponentTeam->show();
    }
    else if (state == 99)
    {
        drawPlayAgain();
    }

    SDL_RenderPresent(renderer);
}

void Game::sendData()
{
    std::string message;
    if (serv != nullptr)
    {
        while (!game->waitQueue.empty())
        {
            message = game->waitQueue.front();
            game->waitQueue.pop();
            serv->send(message);
            // if (serv->get() == RECIEVED + message)
            // {
            //     break;
            // }
        }
    }
    else if (client != nullptr)
    {
       while (!game->waitQueue.empty())
        {
            message = game->waitQueue.front();
            game->waitQueue.pop();
            client->send(message);
            // if (client->get() == RECIEVED + message)
            // {
            //     break;;
            // }
        }
    }
}

void Game::clean()
{
    delete gui;

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    SDL_Quit();

    std::cout << "Game cleaned!" << std::endl;
}

void Game::restartGame()
{
    delete myTeam;
    delete opponentTeam;
    delete game_maze;

    play_again_request = false;
    play_again = false;

    game_over = false;

    teamNum = -1;
    opponentTeamNum = -2;

    game_maze = new Maze(13, gui_width, gui_height);

    loadAllTextures();

    state = 2;

    std::cout << "Game Restart!" << std::endl;
}
   
void Game::loadAllTextures()
{
    game_maze->wall = loadTexture("assets/images/wall.png", renderer);
    if (game_maze->wall == NULL) {std::cout << "Failed loading texture" << std::endl;}

    game_maze->grass = loadTexture("assets/images/grass.jpg", renderer);
    if (game_maze->grass == NULL) {std::cout << "Failed loading texture" << std::endl;}

    win = loadTexture("assets/images/Winner.png", renderer);
    if (win == NULL) {std::cout << "Failed loading texture" << std::endl;}

    lose = loadTexture("assets/images/Loser.png", renderer);
    if (lose == NULL) {std::cout << "Failed loading texture" << std::endl;}

    button = loadTexture("assets/images/tile.jpg", renderer);
    if (button == NULL) {std::cout << "Failed loading texture" << std::endl;}

    play_again_text = text(renderer, "Play Again", play_again_box.h / 5);
    waiting_text = text(renderer, "Waiting for Other Player...", 40 * scale);
    play_again_request_text = text(renderer, "Other Player want to Play Again...", 40 * scale);
}

void Game::drawMazeLoad()
{
    backgroundImage(renderer, game_maze->grass);

    color(renderer, 0);
    static SDL_Texture *mazeLoading = text(renderer, "Maze Loading...", font_size);

    imageCenter(renderer, mazeLoading, gui_width / 2, gui_height / 2);

    color(renderer, 0);
    rectCenter(renderer, gui_width / 2, gui_height / 2 + loading_height, loading_width, loading_height, 1, false);

    for (int i = 0; i < game_maze->n; i++)
    {
        if (i < curRowToSend)
        {
            color(renderer, 0, 255, 0);
            rectCenter(renderer, gui_width / 2 - loading_width / 2 + loading_height / 2 + i * loading_height, gui_height / 2 + loading_height, loading_height, loading_height, 0.8, true);
            color(renderer, 0);
            rectCenter(renderer, gui_width / 2 - loading_width / 2 + loading_height / 2 + i * loading_height, gui_height / 2 + loading_height, loading_height, loading_height, 0.9, false);
        }
    }

}

void Game::drawPlayAgain()
{
    if (!play_again && !play_again_request)
    {
        backgroundImage(renderer, game_maze->grass);
        if (!myTeam->characters[0][0]->dead)
        {
            imageCenter(renderer, win, NULL, gui_width / 2, gui_height / 2, gui_width / 3, gui_height / 7);
        }
        else
        {
            imageCenter(renderer, lose, NULL, gui_width / 2, gui_height / 2, gui_width / 3, gui_height / 7);
        }

        color(renderer, 0, 255, 0, 255);
        imageCenter(renderer, button, NULL, play_again_box.x + play_again_box.w / 2, play_again_box.y + play_again_box.h / 2, play_again_box.w, play_again_box.h, 1, SDL_FLIP_NONE);
        imageCenter(renderer, play_again_text, NULL, play_again_box.x + play_again_box.w / 2, play_again_box.y + play_again_box.h / 2, play_again_box.x / 2, play_again_box.h, 0.9, SDL_FLIP_NONE);
    }
    else if (play_again)
    {
        backgroundImage(renderer, game_maze->grass);
        imageCenter(renderer, waiting_text, gui_width / 2, gui_height / 2);
    }
    else if (play_again_request)
    {
        backgroundImage(renderer, game_maze->grass);
        imageCenter(renderer, play_again_request_text, gui_width / 2, gui_height / 2); 

        color(renderer, 0, 255, 0, 255);
        imageCenter(renderer, button, NULL, play_again_box.x + play_again_box.w / 2, play_again_box.y + play_again_box.h / 2, play_again_box.w, play_again_box.h, 1, SDL_FLIP_NONE);
        imageCenter(renderer, play_again_text, NULL, play_again_box.x + play_again_box.w / 2, play_again_box.y + play_again_box.h / 2, play_again_box.x / 2, play_again_box.h, 0.9, SDL_FLIP_NONE);  
    }
}

void sendMessage(std::string message)
{
    game->waitQueue.push(message);
}

void respond(std::string response)
{
    std::string code = response.substr(0, 2);
    std::string data = response.substr(2);

    if (code == TEAM_SELECT)
    {
        opponentTeamNum = std::stoi(data);
    }
    else if (code == MAZE_STRUCT)
    {
        data = data.substr(2);
        for (int i = 0; i < game->game_maze->n; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                game->game_maze->maze[game->game_maze->n - 1 - game->curRowToSend][game->game_maze->n - 1 - i][(j + 2) % 4] = std::stoi(data.substr(i * 4 + j, 1));
            }
        }
        game->curRowToSend++;
        sendMessage(READY+std::string("0"));
    }
    else if (code == READY)
    {
        game->opponentReady = true;
    }
    else if (code == DEPLOY)
    {
        if (data.substr(0, 1) == "0")
        {
            game->myTeam->opponentKingDeployed = true;
        } 
        game->opponentTeam->deploy(std::stoi(data.substr(0,1)), std::stoi(data.substr(1,1)), std::stoi(data.substr(2,2)), std::stoi(data.substr(4,2)));
    }
    else if (code == MOVEMENT)
    {
        int lvl = std::stoi(data.substr(0, 1));
        int cnt = std::stoi(data.substr(1, 1));
        int dir = std::stoi(data.substr(2, 1));
        game->opponentTeam->characters[lvl][cnt]->setVel((dir + 2) % 4);
    }
    else if (code == ATTACK)
    {
        int attack_lvl = std::stoi(data.substr(0, 1));
        int attack_cnt = std::stoi(data.substr(1, 1));
        int power = std::stoi(data.substr(2, 2));
        game->myTeam->characters[attack_lvl][attack_cnt]->attack(power);

        int lvl = std::stoi(data.substr(4, 1));
        int cnt = std::stoi(data.substr(5, 1));
        int dir = std::stoi(data.substr(6, 1));
        game->opponentTeam->characters[lvl][cnt]->turn((dir + 2) % 4);
    }
    else if (code == DIE)
    {

    }
    else if (code == TURN)
    {
        int lvl = std::stoi(data.substr(0, 1));
        int cnt = std::stoi(data.substr(1, 1));
        int dir = std::stoi(data.substr(2, 1));
        game->opponentTeam->characters[lvl][cnt]->turn((dir + 2) % 4);
    }
    else if (code == BREAK_WALL)
    {
        int i = game->game_maze->n - 1 - std::stoi(data.substr(0, 2));
        int j = game->game_maze->n - 1 - std::stoi(data.substr(2, 2));
        int dir = (2 + std::stoi(data.substr(4, 1))) % 4;
        int pow = std::stoi(data.substr(5, 2));

        int lvl = std::stoi(data.substr(7, 1));
        int cnt = std::stoi(data.substr(8, 1));
        int dirr = std::stoi(data.substr(9, 1));

        game->opponentTeam->characters[lvl][cnt]->turn((dirr + 2) % 4);
        game->myTeam->attackWall(i, j, dir, pow);
    }
    else if (code == PLAY_AGAIN)
    {
        if (std::stoi(data) == 1)
        {
            game->play_again_request = true;
            if (game->play_again && game->play_again_request)
            {
                game->restart = true;
            }
        }
    }
    else if (code == END_GAME)
    {
        game->isRunning = false;
    }
    else
    {
        
    }
}