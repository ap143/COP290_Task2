#ifndef GUI
#define GUI

#include "./utils/draw.hpp"
#include "network.hpp"
#include <vector>
#include <string>

extern Server *serv;
extern Client *client;

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
    const Uint8 textBoxColor[4] = {255, 255, 255, 255};
    std::string live_text;

    const unsigned int maxUserNameLength = 12;

    const float textBoxWidth = gui_width / 3;
    const float textBoxHeight = textBoxWidth / maxUserNameLength;
    const float live_text_font = textBoxHeight;

    SDL_Texture *live_text_texture;
    SDL_Texture *host_button;
    SDL_Texture *join_button;
    SDL_Texture *user_name_message;

    const float buttonWidth = (float) gui_width / 8;
    const float buttonHeight = buttonWidth / 2;
    const float buttonMargin = buttonWidth * 2;
    const float buttonPosY = (float) gui_height * 2 / 3;

    bool onButtonHost;
    bool onButtonJoin;

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

    const float text_size = live_text_font;

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

    bool isHost;
    bool abort;
    SDL_Thread *connectionThread = nullptr;
    SDL_Thread *runnerThread = nullptr;

};

#endif