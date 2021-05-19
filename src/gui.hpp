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
    SDL_Window *window;
    SDL_Renderer *renderer;

    // Show function for different states of game
    void show_username();
    void show_codecheck();
    void show_teamselect();
    void show_randomplace();

    // Handle event func for different states of game
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

    // Initial State Variables //

    const Uint8 textBoxColor[4] = {255, 255, 255, 255};
    std::string live_text;

    const unsigned int maxUserNameLength = 12;

    // UserName text box dimensions
    const float textBoxWidth = gui_width / 3;
    const float textBoxHeight = textBoxWidth / maxUserNameLength;
    const float live_text_font = textBoxHeight;

    // Button Textures
    SDL_Texture *live_text_texture = nullptr;
    SDL_Texture *host_button = nullptr;
    SDL_Texture *join_button = nullptr;
    SDL_Texture *user_name_message = nullptr;

    // Button Dimensions
    const float buttonWidth = (float) gui_width / 8;
    const float buttonHeight = buttonWidth / 2;
    const float buttonMargin = buttonWidth * 2;
    const float buttonPosY = (float) gui_height * 2 / 3;

    // handle event variables for button click
    bool onButtonHost;
    bool onButtonJoin;

    
    // Connection variables //

    // game code for connecting client to host
    std::string passCode;

    SDL_Texture *pass_code_texture = nullptr;
    SDL_Texture *waiting_texture = nullptr;
    SDL_Texture *input_code = nullptr;

    // if host connected to client
    bool connected;


    // Team Selection Variables //

    int const totalTeams = 10;

    // textures for team image, player names, player count, and for team is selected by player.
    std::vector<SDL_Texture*> teams = {};
    
    std::vector<SDL_Texture*> player_names = {};

    std::vector<SDL_Texture*> player_counts = {};

    SDL_Texture* selected = nullptr;

    // Texts Textures
    SDL_Texture* power = nullptr;
    SDL_Texture* health = nullptr;
    SDL_Texture* speed = nullptr;
    SDL_Texture* heal = nullptr;

    // Current Team in team Selection
    int currTeam = 0;

    // Dimensions of team image in team selction state 
    const float centreBoxLength = 300 * scale_width;
    const float sideBoxLength = 150 * scale_height;

    int text_size = live_text_font;

    // properties bar dimensions
    const float bar_length = gui_width / 16;
    const float bar_width = live_text_font / 10;

    // Max values of each properties
    float max_power = 10;
    float max_health = 3000;
    float max_speed = 10;
    float max_heal = 3;

    // Animation variables
    float animate_extent = 0;
    int animate_direction = 0;
    float max_extent = 20;

    // Number of players of each level
    std::string num_players[4] = {"x1", "x4", "x2", "x1"};

    // Players names in each team
    std::string players[10][4] = {{"Thor Odinson", "Groot", "Loki", "Hulk"},  {"Iron Man", "Agents of Sheild", "Vision", "Spiderman"}, {"Captain America", "Antman", "Hawkeye", "Falcon"}, {"Thanos", "Hydra", "Red Skull", "Ultron"}, {"Doctor Strange", "Quicksilver", "Natasha", "Wanda"}, {"Captain Marvel", "Skrulls", "Nick Fury", "Monica"}, {"Black Panther", "Dora Milaje", "War machine", "White Wolf"}, {"Jean Grey", "Cyclops", "Storm", "Logan"}, {"Gamora", "Drax", "Rocket", "Star Lord"}, {"Magneto", "Iceman", "Deadpool", "Mystique"}};

    // Runner thread function
    static int runGame(void* a);

public:
    // Background for current state
    SDL_Texture *gui_bg;

    Gui(SDL_Window *window, SDL_Renderer *renderer);
    ~Gui();

    // Event Handling for state < 4
    void event(SDL_Event event, int &state);

    // Rendered function for state < 4
    void show(int state);

    // true for host
    bool isHost;

    // abort if windows is closed
    bool abort;

    // Thread for connecting with client
    SDL_Thread *connectionThread = nullptr;

    // Thread for sending and responding to client
    SDL_Thread *runnerThread = nullptr;

};

#endif