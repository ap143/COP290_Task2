#ifndef GUI
#define GUI

#include "SDL.h"
#include "./utils/draw.hpp"
#include "network.hpp"
#include <vector>
#include <string>

extern Server *serv;
extern Client *client;

extern std::string user_name;
extern std::string opponent_name;

extern float scale_width;
extern float scale_height;
extern float scale;

extern int gui_width;
extern int gui_height;

extern int teamNum;
extern int opponentTeamNum;

class Gui
{

private:
    SDL_Texture *gui_bg;

    SDL_Window *window;
    SDL_Renderer *renderer;

    void show_username();
    void show_codecheck();
    void show_teamselect();
    void show_randomplace();

    void event_username(SDL_Event event, int &state);
    void event_codecheck(SDL_Event event, int &state);
    void event_teamselect(SDL_Event event, int &state);
    void event_randomplace(SDL_Event event, int &state);

    void setInfo();
    static int initialConnection(void* a);
    void reel(bool direction);

    // Username properties
    Uint8 textBoxColor[4] = {255, 255, 255, 255};
    std::string live_text;
    const float live_text_font = 48 * scale;
    const float textBoxWidth = 480 * scale_width;
    const float textBoxHeight = 48 * scale_height;
    
    int maxUserNameLength = 12;

    SDL_Texture *live_text_texture;
    SDL_Texture *host_button;
    SDL_Texture *join_button;
    SDL_Texture *user_name_message;

    const float buttonWidth = 240 * scale_width;
    const float buttonHeight = 72 * scale_height;
    const float buttonMargin = 150 * scale_width;
    const float buttonPosY = 480 * scale_height;

    bool onButtonHost;
    bool onButtonJoin;

    bool isHost;
    std::string passCode;

    // Connection variables

    SDL_Texture *pass_code_texture;
    SDL_Texture *waiting_texture;
    SDL_Texture *input_code;

    bool connected;

    // Team Selection Variables
    int const totalTeams = 10;

    std::vector<SDL_Texture*> teams;
    std::vector<SDL_Texture*> player_names;
    SDL_Texture* selected;

    int currTeam = 0;
    const float centreBoxLength = 300 * scale_width;
    const float sideBoxLength = 150 * scale_height;

    const float text_size = 48 * scale;

    bool movedLeft;
    bool movedRight;
    bool animate = false;

    std::string players[10][4] = {{"Team Thor", "Groot", "Loki", "Hulk"},  {"Team Iron Man", "Agents of Sheild", "Vision", "Spiderman"}, {"Team Captain America", "Antman", "Hawkeye", "Falcon"}, {"Team Thanos", "Hydra", "Red Skull", "Ultron"}, {"Team Doctor Strange", "Pietro", "Natasha", "Wanda"}, {"Team Captain MArvel", "Skrulls", "Nick Fury", "Monica"}, {"Team Black Panther", "Dora Milaje", "War machine", "White Wolf"}, {"Team Jean", "Cyclops", "Storm", "Logan"}, {"Team Gamora", "Drax", "Rocket", "Star Lord"}, {"Team Magneto", "Iceman", "Deadpool", "Mystique"}};

    // Runner thread function
    static int runGame(void* a);

public:
    Gui(SDL_Window *window, SDL_Renderer *renderer);

    ~Gui();

    void event(SDL_Event event, int &state);

    void show(int state);

    bool abort;
    SDL_Thread *connectionThread = nullptr;
    SDL_Thread *runnerThread = nullptr;

    void sendMessage(std::string message);

};

#endif