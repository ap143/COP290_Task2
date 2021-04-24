#include "gui.hpp"

Gui::Gui(SDL_Window* window, SDL_Renderer* renderer)
{
    this->window = window;
    this->renderer = renderer;
}

void Gui::event(SDL_Keycode keyCode, int& state)
{
    switch (state)
    {
        case 0:
            event_username(keyCode, state);
            break;
        case 1:
            event_codecheck(keyCode, state);
            break;
        case 2:
            event_teamselect(keyCode, state);
            break;
        case 3:
            event_randomplace(keyCode, state);
    }
}

void Gui::show(int state)
{
    switch (state)
    {
        case 0:
            show_username();
            break;
        case 1:
            show_codecheck();
            break;
        case 2:
            show_teamselect();
            break;
        case 3:
            show_randomplace();
    }
}


