#ifndef GAME
#define GAME

#include "SDL.h"
#include <iostream>
#include "./utils/draw.hpp"
#include <vector>
#include <map>
#include "gui.hpp"
#include "character.hpp"
#include "maze.hpp"

class Game
{

public:
    Game();
    ~Game();

    static std::map<int, properties> charProp;
    static std::map<int, std::vector<std::string>> textureMap;
    static std::vector<Character> chars;

    void init(const char *title, int xpos, int ypos, int width, int height);

    void handleEvents();
    void update();
    void render();
    void clean();

    bool running() { return isRunning; };

    void loadAllTextures();

    void loadCharacters(int teamA, int teamB);

private:
    bool isRunning;
    Maze *game_maze;
    SDL_Window *window;
    SDL_Renderer *renderer;

    Gui *gui;

    int keySpeedLimit = 0;
    int state;
    /*
    *   0: Get user name
    *   1: Generate/Check passcode => connect
    *   2: Connected state: Select team => Load all character textures and make objects
    *   3: Show random place to match => Generate maze and draw it
    *   4: Ready to play => Countdown to 3
    *   5: Game start
    *   6: Game ends/one player quits => Display winner and new game option
    */
};

#endif
