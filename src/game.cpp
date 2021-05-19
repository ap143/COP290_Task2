#include "game.hpp"

Game::Game()
{
    game_maze = new Maze(13, gui_width, gui_height);
    state = 0;

    loading_height = loading_width / game_maze->n;
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

    loadAllTextures();
    loadAllSounds();

    isRunning = true;
}

void Game::handleEvents()
{

    SDL_FlushEvent(SDL_MOUSEMOTION);
    SDL_FlushEvent(SDL_KEYDOWN);

    SDL_Event event;
    SDL_PollEvent(&event);

    if (event.type == SDL_QUIT)
    {
        if (state >= 2)
        {
            sendMessage(END_GAME + std::string("0"));
        }
        isRunning = false;
        state = -1;
        return;
    }

    if (event.type == SDL_KEYDOWN && (SDL_GetModState() & (SDLK_RSHIFT | SDLK_LSHIFT)) && event.key.keysym.sym == SDLK_m)
    {
        if (Mix_VolumeMusic(0) == 0)
        {
            Mix_VolumeMusic(MIX_MAX_VOLUME);
        }
        else
        {
            Mix_VolumeMusic(0);
        }

        if (Mix_Volume(0, 0) == 0)
        {
            Mix_Volume(-1, MIX_MAX_VOLUME);
        }
        else
        {
            Mix_Volume(-1, 0);
        }
    }

    if (state < 4)
    {
        gui->event(event, state);
    }
    else if (state == 7)
    {
        myTeam->handleEvent(event);
        myScore->handleEvent(event);
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

        myScore = new Score(renderer, game_maze);

        state++;
    }
    else if (state == 7)
    {
        if (game->myScore->play_again && game->myScore->play_again_request)
        {
            restartGame();
            return;
        }

        if (myScore->king_deploy_time == 0 && !myTeam->kingDeployed)
        {
            deployKing();
        }

        myTeam->update();
        opponentTeam->update();

        myScore->kingDeployed = myTeam->kingDeployed & myTeam->opponentKingDeployed;

        if (game_over && myTeam->characters[0][0]->dead)
        {
            myScore->win = false;
        }
        else if (game_over)
        {
            myScore->win = true;
        }

        myScore->update();
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
        myScore->show();
    }
    else if (state == 99)
    {
        drawExitState();
    }

    SDL_RenderPresent(renderer);
}

void Game::playSound()
{
    if (state < 4)
    {
        if (!Mix_PlayingMusic())
        {
            Mix_PlayMusic(bgm, -1);
        }
    }
    else if (state < 7)
    {
        Mix_HaltMusic();
    }
    else if (state == 7)
    {
        if (!Mix_PlayingMusic())
        {
            Mix_PlayMusic(war_music, -1);
        }
    }
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
        }
    }
    else if (client != nullptr)
    {
        while (!game->waitQueue.empty())
        {
            message = game->waitQueue.front();
            game->waitQueue.pop();
            client->send(message);
        }
    }
}

void Game::clean()
{
    if (gui->isHost && serv != nullptr)
    {
        serv->end();
    }
    else if (!gui->isHost && client != nullptr)
    {
        client->end();
    }

    if (myTeam != nullptr) delete myTeam;
    if (opponentTeam != nullptr) delete opponentTeam;
    
    delete game_maze;
    
    if (myScore != nullptr) delete myScore;
    if (gui != nullptr) delete gui;

    SDL_DestroyTexture(exit_text);
    SDL_DestroyTexture(exit_image);

    Mix_HaltMusic();
    Mix_HaltChannel(-1);

    Mix_FreeMusic(bgm);
    Mix_FreeMusic(war_music);

    Mix_FreeChunk(enemy_deploy);
    Mix_FreeChunk(king_deploy);
    Mix_FreeChunk(friend_deploy);
    Mix_FreeChunk(king_die);

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    Mix_Quit();
    TTF_Quit();
    SDL_Quit();

    std::cout << "Game cleaned!" << std::endl;
}

void Game::restartGame()
{
    if (myTeam != nullptr) delete myTeam;
    if (opponentTeam != nullptr) delete opponentTeam;
    
    delete game_maze;
    
    if (myScore != nullptr) delete myScore;

    game_over = false;

    teamNum = -1;
    opponentTeamNum = -2;

    game_maze = new Maze(13, gui_width, gui_height);

    loadAllTextures();

    state = 2;

    if (Mix_PlayingMusic())
    {
        Mix_HaltMusic();
    }

    Mix_PlayMusic(bgm, -1);

    std::cout << "Game Restart!" << std::endl;
}

void Game::loadAllSounds()
{
    bgm = Mix_LoadMUS("./assets/sounds/bg_music.mp3");
    war_music = Mix_LoadMUS("./assets/sounds/war_music.mp3");

    enemy_deploy = Mix_LoadWAV("./assets/sounds/enemy_deploy.wav");
    friend_deploy = Mix_LoadWAV("./assets/sounds/friend_deploy.wav");
    king_deploy = Mix_LoadWAV("./assets/sounds/king_deploy.wav");
    king_die = Mix_LoadWAV("./assets/sounds/king_die.wav");
}
   
void Game::loadAllTextures()
{
    exit_image = loadTexture("assets/images/exit.png", renderer);

    game_maze->wall = loadTexture("assets/images/wall.png", renderer);
    if (game_maze->wall == NULL) {std::cout << "Failed loading texture" << std::endl;}

    game_maze->grass = loadTexture("assets/images/grass.jpg", renderer);
    if (game_maze->grass == NULL) {std::cout << "Failed loading texture" << std::endl;}
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

void Game::drawExitState()
{
    if (exit_text == nullptr)
    {
        color(renderer, 255);
        exit_text = text(renderer, opponent_name + " doesn't wants to play... *LUDO Vibes* ;-)", 36 * scale);
    }

    backgroundImage(renderer, exit_image);
    imageCenter(renderer, exit_text, gui_width / 2, gui_height / 2);
}

void Game::deployKing()
{
    int i = game_maze->n - 1;
    int j = game_maze->n - 1;
    myTeam->characters[0][0]->deploy(i, j);

    sendMessage(DEPLOY + std::string("00") + ((i < 10) ? "0" : "") + std::to_string(i) + ((j < 10) ? "0" : "") + std::to_string(j));
    
    myTeam->activeLevel = -1;
    myTeam->deployingNow = false;
    myTeam->count[0] = 0;
    myTeam->kingDeployed = true;

    if(myTeam->count_text[0] != NULL)
    {
        SDL_DestroyTexture(myTeam->count_text[0]);
    }
    
    myTeam->count_text[0] = text(renderer, "x0", myTeam->count_text_size);
}

void sendMessage(std::string message)
{
    if (!recieving_messages)
    {
        return;
    }
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
        game->opponentTeam->deploy(std::stoi(data.substr(0,1)), std::stoi(data.substr(1,1)), std::stoi(data.substr(2,2)), std::stoi(data.substr(4,2)));
        if (data.substr(0, 1) == "0")
        {
            game->myTeam->opponentKingDeployed = true;
        }
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

        int attackDir = std::stoi(data.substr(7, 1));
        game->opponentTeam->characters[lvl][cnt]->attackDir = attackDir == 4 ? 4 : (attackDir + 2) % 4;
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

        int attackDir = std::stoi(data.substr(10, 1));
        game->opponentTeam->characters[lvl][cnt]->attackDir = attackDir == 4 ? 4 : (attackDir + 2) % 4;
    }
    else if (code == PLAY_AGAIN)
    {
        if (std::stoi(data) == 1)
        {
            game->myScore->play_again_request = true;
        }
    }
    else if (code == END_GAME)
    {
        game_over = true;
        recieving_messages = false;
        game->state = 99;
    }
    else
    {
        
    }
}