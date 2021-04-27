#include "game.hpp"

std::map<int, properties> Game::charProp = 
{
    std::make_pair(0, properties(5, 12, 8, 2)),
    std::make_pair(1, properties(2, 5, 4, 6)),
    std::make_pair(2, properties(4, 8, 7, 4)),
    std::make_pair(3, properties(6, 10, 5, 3))
};

std::map<int, std::vector<std::string>> Game::textureMap = 
{

};

Game::Game()
{
    game_maze = new Maze();
    state = 0;
}

Game::~Game()
{
}

void Game::init(const char *title, int xpos, int ypos, int width, int height)
{

    isRunning = true;

    TTF_Init();

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        std::cout << "Subsystems initialized..." << std::endl;
    }
    else
    {
        std::cerr << "Error initializing..." << std::endl;
        exit(-1);
    }

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

    game_maze->generate();
    std::cout << "Maze generated..." << std::endl;

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

}

void Game::update()
{
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    if (state < 4)
    {
        gui->show(state);
    }
    else
    {
        game_maze->show(renderer, window);
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

void Game::loadCharacters(int teamA, int teamB)
{
    
}
