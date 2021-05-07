#ifndef GAME
#define GAME

#include "gui.hpp"
#include "teamView.hpp"

class Game
{

public:
    Game();
    ~Game();

    Maze *game_maze;
    int curRowToSend;
    bool opponentReady = false;
    bool isRunning;

    static std::map<int, properties> charProp;

    std::queue<std::string> waitQueue;

    Teamview *myTeam;
    Teamview *opponentTeam;

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
    void sendData();

    bool running() { return isRunning; };

    void loadAllTextures();

private:

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
