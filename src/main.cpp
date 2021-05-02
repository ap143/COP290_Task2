#include "game.hpp"
#include <iostream>
#include "utils/helper.hpp"

Game* game = nullptr;

Server* serv;
Client* client;

std::string user_name;
std::string oponent_name;

int gui_width = 4;
int gui_height = 3;

int teamNum = -1;
int oppenentNum = -1;

int main(int argc, char* argv[])
{

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

    SDL_DisplayMode DM;
    SDL_GetDesktopDisplayMode(0, &DM);

    gui_width = gui_width * DM.w * 240 / 1920;
    gui_height = gui_height * DM.h * 240 / 1080;

    const int FPS = 30;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    game = new Game();

    game->init("Maze Wars", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, gui_width, gui_height);

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
