#include "game.hpp"

Game::Game()
{
}

Game::~Game()
{
}

void Game::init(const char *title, int xpos, int ypos, int width, int height)
{

    isRunning = true;

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
        std::cout << "Rendere created..." << std::endl;
    }

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

    

    SDL_RenderPresent(renderer);
}

void Game::clean()
{

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    SDL_Quit();

    std::cout << "Game cleaned!" << std::endl;

}
