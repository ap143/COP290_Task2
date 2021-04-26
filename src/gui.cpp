#include "gui.hpp"

Gui::Gui(SDL_Window *window, SDL_Renderer *renderer)
{
    this->window = window;
    this->renderer = renderer;

    this->gui_bg = loadTexture("./assets/images/bg.png", this->renderer);

    this->live_text = "";

    this->live_text_texture = text(renderer, live_text, live_text_font);
}

void Gui::event(SDL_Event event, int &state)
{
    switch (state)
    {
    case 0:
        event_username(event, state);
        break;
    case 1:
        event_codecheck(event, state);
        break;
    case 2:
        event_teamselect(event, state);
        break;
    case 3:
        event_randomplace(event, state);
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

void Gui::show_username()
{
    backgroundImage(renderer, gui_bg);

    color(renderer, 255);

    rect(renderer, gui_width / 2 - textBoxWidth / 2, 2 * gui_height / 3 - textBoxHeight / 2, textBoxWidth, textBoxHeight, true);

    color(renderer, 0);

    imageCenter(renderer, live_text_texture, gui_width / 2, 2 * gui_height / 3);
}

void Gui::show_codecheck()
{
}

void Gui::show_teamselect()
{
}

void Gui::show_randomplace()
{
}

void Gui::event_username(SDL_Event event, int &state)
{

    if (event.key.keysym.sym == SDLK_BACKSPACE)
    {
        if (live_text.length() > 0)
        {
            live_text = live_text.substr(0, live_text.length() - 1);
            SDL_DestroyTexture(live_text_texture);
            live_text_texture = text(renderer, live_text, live_text_font);
        }
    }
    else if (event.key.keysym.scancode == SDL_SCANCODE_RETURN)
    {
        user_name = live_text;
        state++;
    }
    else if (event.type = SDL_TEXTINPUT)
    {
        live_text += event.text.text;
        SDL_DestroyTexture(live_text_texture);
        live_text_texture = text(renderer, live_text, live_text_font);
    }
}

void Gui::event_codecheck(SDL_Event event, int &state)
{
}

void Gui::event_teamselect(SDL_Event event, int &state)
{
}

void Gui::event_randomplace(SDL_Event event, int &state)
{
}
