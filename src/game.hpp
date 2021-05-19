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

    // Maze of current game play
    Maze *game_maze;

    // Maze Row while sending data to client
    int curRowToSend;

    // Opponent ready when received maze
    bool opponentReady = false;

    // Game is running
    bool isRunning;

    // If game is to be restarted
    bool restart = false;

    std::queue<std::string> waitQueue;

    // Team view object of my and opponent team
    Teamview *myTeam = nullptr;
    Teamview *opponentTeam = nullptr;

    // Scoreboard object
    Score *myScore = nullptr;

    int state;
    /*
    *   0: Get user name
    *   1: Generate/Check passcode => connect
    *   2: Connected state: Select team => Load all character textures and make objects
    *   4: Generate Maze
    *   5: Send Maze
    *   6: Load characters
    *   7: Game play
    *   99: Exit state
    */

    void init(const char *title, int xpos, int ypos, int width, int height);

    // Game controllers according to states
    void handleEvents();
    void update();
    void render();
    void playSound();

    void clean();

    // Sends the message queue to the opponent
    void sendData();

    bool running() { return isRunning; };

private:

    SDL_Window *window;
    SDL_Renderer *renderer;

    Gui *gui;

    // Maze load display
    void drawMazeLoad();
    // Loading bar dimensions
    float font_size = 24 * scale;
    float loading_width = gui_width * 2 / 3;
    float loading_height;

    // Force deploy of king
    void deployKing();

    // Deleting all objects and start game from team selection -> state = 2
    void restartGame();

    // A player leaves game
    void drawExitState();
    // Background image and text in exit state
    SDL_Texture *exit_text = nullptr;
    SDL_Texture *exit_image = nullptr;


    // Loads all textures
    void loadAllTextures();

    // Loads all sounds for game
    void loadAllSounds();

};

extern Game *game;

#endif
