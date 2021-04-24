#include "game.hpp"
#include <iostream>
#include "utils/helper.hpp"
#include "start.hpp"

Game* game = nullptr;

Server* serv;
Client* client;

std::string user_name;
std::string oponent_name;

int main(int argc, char* argv[])
{

    // if (start(argc, argv) == 0)
    // {
    //     return 0;
    // }

    // start(argc, argv);

    const int FPS = 30;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    game = new Game();

    game->init("Maze Wars", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600);

    while (game->running())
    {
        frameStart = SDL_GetTicks();

        game->handleEvents();
        game->update();
        game->render();

        frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay-frameTime);
        }
    }

    game->clean();

    return 0;
}
