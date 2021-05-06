#include "game.hpp"

std::map<int, properties> Game::charProp = 
{
    std::make_pair(0, properties(5, 12, 8, 2)),
    std::make_pair(1, properties(2, 5, 4, 6)),
    std::make_pair(2, properties(4, 8, 7, 4)),
    std::make_pair(3, properties(6, 10, 5, 3))
};


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

    gui = new Gui(window, renderer);
    loadAllTextures();

    isRunning = true;
}

void Game::handleEvents()
{

    SDL_FlushEvent(SDL_MOUSEMOTION);

    SDL_Event event;
    SDL_PollEvent(&event);

    if (event.type == SDL_QUIT)
    {
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
        std::cout << "state 6" << std::endl;
        myTeam = new Teamview(renderer, game_maze, teamNum, true);
        opponentTeam = new Teamview(renderer, game_maze, opponentTeamNum, false);
        myTeam->enemyTeam = opponentTeam;
        opponentTeam->enemyTeam = myTeam;
        state++;
    }else if (state == 7)
    {
        myTeam->update();
        opponentTeam->update();
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
    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    delete gui;

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    SDL_Quit();

    std::cout << "Game cleaned!" << std::endl;
}

void Game::loadAllTextures()
{
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

void sendMessage(std::string message)
{
    bool busy = false;

    game->waitQueue.push(message);

    if (busy)
    {
        return;
    }

    busy = true;

    if (serv != nullptr)
    {
        while (!game->waitQueue.empty())
        {
            message = game->waitQueue.front();
            game->waitQueue.pop();
            serv->send(message);
            if (serv->get() == RECIEVED + message)
            {
                break;
            }
        }
    }
    else if (client != nullptr)
    {
       while (!game->waitQueue.empty())
        {
            message = game->waitQueue.front();
            game->waitQueue.pop();
            client->send(message);
            if (client->get() == RECIEVED + message)
            {
                break;;
            }
        }
    }

    busy = false;
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
        int row = std::stoi(data.substr(0, 2));
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
        int level = std::stoi(data.substr(0, 1));
        int cnt = std::stoi(data.substr(1, 1));
        int dir = std::stoi(data.substr(2, 1));
        game->opponentTeam->characters[level][cnt]->setVel((dir + 2) % 4);
    }
    else if (code == ATTACK)
    {
        int level = std::stoi(data.substr(0, 1));
        int cnt = std::stoi(data.substr(1, 1));
        int power = std::stoi(data.substr(2, 1));
        game->myTeam->characters[level][cnt]->attack(power);
    }
    else if (code == DIE)
    {
        
    }
    else if (code == TURN)
    {
        int level = std::stoi(data.substr(0, 1));
        int cnt = std::stoi(data.substr(1, 1));
        int dir = std::stoi(data.substr(2, 1));
        game->opponentTeam->characters[level][cnt]->turn((dir + 2) % 4);
    }
    else if (code == BREAK_WALL)
    {
        int i = std::stoi(data.substr(0, 2));
        int j = std::stoi(data.substr(2, 2));
        int dir = std::stoi(data.substr(4, 1));
        int pow = std::stoi(data.substr(5, 1));
        game->myTeam->attackWall(i, j, dir, pow);
    }
    else if (code == END_GAME)
    {

    }
    else
    {
        
    }

    if (serv != nullptr)
    {
        serv->send(RECIEVED + response);
    }
    else if (client != nullptr)
    {
        client->send(RECIEVED + response);
    }
}