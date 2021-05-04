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

    Maze *game_maze;
    int curRowToSend;
    bool opponentReady = false;

    static std::map<int, properties> charProp;
    static std::map<int, std::vector<std::string>> textureMap;
    static std::vector<Character> chars;

    int state;
    /*
    *   0: Get user name
    *   1: Generate/Check passcode => connect
    *   2: Connected state: Select team => Load all character textures and make objects
    *   3: Show random place to match => Generate maze and draw it
    *   4: Generate Maze
    *   5: Send Maze
    *   6: Load characters
    */

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
    
    SDL_Window *window;
    SDL_Renderer *renderer;

    Gui *gui;
 
    void drawMazeLoad();
    const float font_size = 24 * scale;
    const float loading_width = gui_width * 2 / 3;
    const float loading_height = loading_width / 20;

};

extern Game *game;

#endif
