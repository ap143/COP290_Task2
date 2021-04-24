#ifndef GUI
#define GUI

#include "SDL.h"
#include "./utils/draw.hpp"

class Gui
{

private:

    SDL_Window* window;
    SDL_Renderer* renderer;

    void show_username();
    void show_codecheck();
    void show_teamselect();
    void show_randomplace();

    void event_username(SDL_Keycode keyCode, int& state);
    void event_codecheck(SDL_Keycode keyCode, int& state);
    void event_teamselect(SDL_Keycode keyCode, int& state);
    void event_randomplace(SDL_Keycode keyCode, int& state);

public:

    Gui(SDL_Window* window, SDL_Renderer* renderer);

    void event(SDL_Keycode keyCode, int& state);

    void show(int state);

};

#endif