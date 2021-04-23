#include "game.hpp"

Game::Game()
{
    game_maze = new Maze();
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

    loadAllTextures();

    game_maze->generate();
    std::cout << "Maze generated..." << std::endl;

    isRunning = true;
}

void Game::handleEvents()
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT:
        isRunning = false;
        break;
    default:
        break;
    }
}

void Game::update()
{
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    game_maze->show(renderer, window);

    SDL_RenderPresent(renderer);
}

void Game::clean()
{

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
