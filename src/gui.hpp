#ifndef GUI
#define GUI

#include "./utils/draw.hpp"
#include "network.hpp"
#include <vector>
#include <string>
#include <fstream>
#include "character.hpp"

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

    // fonts
    TTF_Font *font1 = nullptr;
    TTF_Font *font2 = nullptr;
    TTF_Font *font3 = nullptr;

    // For animation
    void team_select_update();

    // To set username and code
    void setInfo();

    // Connection thread function
    static int initialConnection(void* a);

    // Username properties
    const Uint8 textBoxColor[4] = {255, 255, 255, 255};
    std::string live_text;

    const unsigned int maxUserNameLength = 12;

    const float textBoxWidth = gui_width / 3;
    const float textBoxHeight = textBoxWidth / maxUserNameLength;
    const float live_text_font = textBoxHeight;

    SDL_Texture *live_text_texture = nullptr;
    SDL_Texture *host_button = nullptr;
    SDL_Texture *join_button = nullptr;
    SDL_Texture *user_name_message = nullptr;

    const float buttonWidth = (float) gui_width / 8;
    const float buttonHeight = buttonWidth / 2;
    const float buttonMargin = buttonWidth * 2;
    const float buttonPosY = (float) gui_height * 2 / 3;

    bool onButtonHost;
    bool onButtonJoin;

    std::string passCode;

    // Connection variables

    SDL_Texture *pass_code_texture = nullptr;
    SDL_Texture *waiting_texture = nullptr;
    SDL_Texture *input_code = nullptr;

    bool connected;

    // Team Selection Variables
    int const totalTeams = 10;

    std::vector<SDL_Texture*> teams = {};
    
    std::vector<SDL_Texture*> player_names = {};

    std::vector<SDL_Texture*> player_counts = {};

    SDL_Texture* selected = nullptr;

    SDL_Texture* power = nullptr;
    SDL_Texture* health = nullptr;
    SDL_Texture* speed = nullptr;
    SDL_Texture* heal = nullptr;

    int currTeam = 0;
    const float centreBoxLength = 300 * scale_width;
    const float sideBoxLength = 150 * scale_height;

    const float text_size = live_text_font;

    const float bar_length = gui_width / 16;
    const float bar_width = text_size / 10;
    int max_power = 10;
    int max_health = 3000;
    int max_speed = 10;
    int max_heal = 10;

    bool movedLeft;
    bool movedRight;
    
    float animate_extent = 0;
    int animate_direction = 0;
    float max_extent = 20;

    std::string num_players[4] = {"x1", "x4", "x2", "x1"};
    std::string players[10][4] = {{"Thor Odinson", "Groot", "Loki", "Hulk"},  {"Iron Man", "Agents of Sheild", "Vision", "Spiderman"}, {"Captain America", "Antman", "Hawkeye", "Falcon"}, {"Thanos", "Hydra", "Red Skull", "Ultron"}, {"Doctor Strange", "Quicksilver", "Natasha", "Wanda"}, {"Captain Marvel", "Skrulls", "Nick Fury", "Monica"}, {"Black Panther", "Dora Milaje", "War machine", "White Wolf"}, {"Jean Grey", "Cyclops", "Storm", "Logan"}, {"Gamora", "Drax", "Rocket", "Star Lord"}, {"Magneto", "Iceman", "Deadpool", "Mystique"}};

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