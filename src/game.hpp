#ifndef GAME
#define GAME

#include "gui.hpp"
#include "teamView.hpp"
#include "score.hpp"

class Game
{

public:
    Game();
    ~Game();

    Maze *game_maze;
    int curRowToSend;
    bool opponentReady = false;
    bool isRunning;

    bool restart = false;

    static std::map<int, properties> charProp;

    std::queue<std::string> waitQueue;

    Teamview *myTeam;
    Teamview *opponentTeam;

    Score *myScore;

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
    void deployKing();
    void restartGame();

    float font_size = 24 * scale;
    float loading_width = gui_width * 2 / 3;
    float loading_height;
};

extern Game *game;

#endif
