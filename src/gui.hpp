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
extern std::string oponent_name;

extern const int gui_width;
extern const int gui_height;

extern int teamNum;
extern int oppenentNum;

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
    const int live_text_font = 32;
    const int textBoxWidth = 320;
    const int textBoxHeight = 32;
    const int maxUserNameLength = 12;

    SDL_Texture *live_text_texture;
    SDL_Texture *host_button;
    SDL_Texture *join_button;
    SDL_Texture *user_name_message;

    const int buttonWidth = 160;
    const int buttonHeight = 48;
    const int buttonMargin = 100;
    const int buttonPosY = 360;

    bool onButtonHost;
    bool onButtonJoin;

    bool isHost;
    std::string passCode;

    // Connection variables

    SDL_Texture *pass_code_texture;
    SDL_Texture *waiting_texture;

    bool connected;

    // Team Selection Variables
    int const totalTeams = 10;

    std::vector<SDL_Texture*> teams;
    std::vector<SDL_Texture*> player_names;

    int currTeam = 0;
    int const centreBoxLength = 175;
    int const sideBoxLength = 100;

    int const text_size = 32;

    bool movedLeft;
    bool movedRight;
    bool animate = false;

    std::string players[10][4] = {{"Team Thor", "Groot", "Loki", "Hulk"},  {"Team Iron Man", "Agents of Sheild", "Vision", "Spiderman"}, {"Team Captain America", "Antman", "Hawkeye", "Falcon"}, {"Team Thanos", "Hydra", "Red Skull", "Ultron"}, {"Team Doctor Strange", "Pietro", "Natasha", "Wanda"}, {"Team Captain MArvel", "Skrulls", "Nick Fury", "Monica"}, {"Team Black Panther", "Dora Milaje", "War machine", "White Wolf"}, {"Jean", "Cyclops", "Storm", "Logan"}, {"Team Gamora", "Drax", "Rocket", "Star Lord"}, {"Team Magneto", "Iceman", "Deadpool", "Mystique"}};
    

public:
    Gui(SDL_Window *window, SDL_Renderer *renderer);

    ~Gui();

    void event(SDL_Event event, int &state);

    void show(int state);

    bool abort;
    SDL_Thread *connectionThread;

};

#endif