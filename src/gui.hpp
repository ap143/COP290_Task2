#ifndef GUI
#define GUI

#include "SDL.h"
#include "./utils/draw.hpp"
#include "network.hpp"

extern Server* serv;
extern Client* client;

extern std::string user_name;
extern std::string oponent_name;

extern const int gui_width;
extern const int gui_height;

class Gui
{

private:

    SDL_Texture* gui_bg;

    SDL_Window* window;
    SDL_Renderer* renderer;

    void show_username();
    void show_codecheck();
    void show_teamselect();
    void show_randomplace();

    void event_username(SDL_Event event, int& state);
    void event_codecheck(SDL_Event event, int& state);
    void event_teamselect(SDL_Event event, int& state);
    void event_randomplace(SDL_Event event, int& state);

    // Username properties
    Uint8 textBoxColor[4] = {255, 255, 255, 255};
    std::string live_text;
    const int live_text_font = 32;
    const int textBoxWidth = 320;
    const int textBoxHeight = 32;

    SDL_Texture* live_text_texture;


public:

    Gui(SDL_Window* window, SDL_Renderer* renderer);

    void event(SDL_Event event, int& state);

    void show(int state);

};

#endif