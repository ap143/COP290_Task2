#include "game.hpp"
#include <iostream>
#include "network.hpp"
#include "utils/helper.hpp"

Game* game = nullptr;
Server* serv;
Client* client;

std::string user_name;
std::string oponent_name;

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        error("Please provide join or host information");
    }

    std::cout << "Input your username: ";
    std::cin >> user_name;

    connectFirst(argv[1]);

    const int FPS = 30;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    game = new Game();

    game->init("Maze Wars", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 1000);

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

    if ((std::string) argv[1] == "1")
    {
        serv->send("end");
        serv->end();
        delete serv;
    }
    else if ((std::string) argv[1] == "0")
    {
        client->send("end");
        client->end();
        delete client;
    }

    return 0;
}

void connectFirst(std::string arg)
{
    // arg = "1"; Host
    // arg = "0"; Join

    if (arg == "1")
    {
        std::cout << "Set a password: ";
        std::string pass;
        std::cin >> pass;

        std::cout << "Waiting for the player..." << std::endl;

        std::string user_pass;

        serv = new Server("127.0.0.1");

        do
        {
            user_pass = serv->get();
            if (user_pass == pass)
            {
                serv->send("1");
                serv->send(user_name);
                oponent_name = serv->get();
                std::cout << "Connected to " << oponent_name << std::endl;
                break;
            }else{
                serv->send("0");
                serv->send("Invalid password, try again");
            }
        } while (true);
    }
    else if (arg == "0")
    {
        client = new Client("127.0.0.1");

        do
        {
            std::cout << "Input the password: ";
            
            std::string pass;
            std::cin >> pass;

            client->send(pass);
            
            std::string response = client->get();

            if (response == "1")
            {
                oponent_name = client->get();
                std::cout << "Connected to " << oponent_name << std::endl;
                client->send(user_name);
                break;
            }else
            {
                response = client->get();
                std::cout << response << std::endl;
            }

        } while (true);
    }
    else
    {
        error("Invalid commandline argument");
    }
}
