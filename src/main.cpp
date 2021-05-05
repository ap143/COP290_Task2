#include "game.hpp"
#include "utils/helper.hpp"

Game* game = nullptr;

Server* serv = nullptr;
Client* client = nullptr;

std::string user_name;
std::string opponent_name;

int gui_width = 4;
int gui_height = 3;

float scale_width, scale_height;
float scale;

int teamNum = -1;
int opponentTeamNum = -2;

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

    scale_width = (float) DM.w / 1920;
    scale_height = (float) DM.h / 1080;
    scale = std::min(scale_height, scale_width);

    gui_width = (int) (gui_width * scale_width) * 300;
    gui_height = (int) (gui_height * scale_height) * 300;

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
