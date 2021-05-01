#include "gui.hpp"

Gui::Gui(SDL_Window *window, SDL_Renderer *renderer)
{
    this->window = window;
    this->renderer = renderer;

    this->gui_bg = loadTexture("./assets/images/bg.png", this->renderer);

    this->live_text = "";

    color(renderer, 0);
    this->live_text_texture = text(renderer, live_text, live_text_font);
    this->host_button = text(renderer, "Host", live_text_font);
    this->join_button = text(renderer, "Join", live_text_font);

    color(renderer, 150);
    this->user_name_message = text(renderer, "Username...", live_text_font * 2 / 3);

    this->onButtonHost = this->onButtonJoin = false;
    
    this->waiting_texture = text(renderer, "Waiting...", live_text_font);

    for (int i = 0; i<10; i++) {
        teams.push_back(loadTexture(("./assets/images/characters/t" + std::to_string(i+1) + ".png").c_str(), this->renderer));
    }

    player_names.push_back(text(renderer, players[currTeam][0], text_size));
    for (int i = 1; i<4; i++) {
        player_names.push_back(text(renderer, players[currTeam][i], text_size / 2));
    }

}

Gui::~Gui()
{
    SDL_DestroyTexture(gui_bg);
    SDL_DestroyTexture(live_text_texture);
    SDL_DestroyTexture(host_button);
    SDL_DestroyTexture(join_button);
    SDL_DestroyTexture(user_name_message);
    SDL_DestroyTexture(pass_code_texture);
    SDL_DestroyTexture(waiting_texture);

    for (int i = 0; i<4; i++)
    {
        SDL_DestroyTexture(player_names[i]);
    }

    for (int i = 0; i<10; i++)
    {
        SDL_DestroyTexture(teams[i]);
    }

    abort = true;
    SDL_WaitThread(connectionThread, NULL);

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
    // Text box
    rectCenter(renderer, gui_width / 2, gui_height / 2, textBoxWidth, textBoxHeight, 1, true);

    // Host box
    if (onButtonHost)
    {
        color(renderer, 255, 239, 99);
        rectCenter(renderer, buttonMargin + buttonWidth / 2, 3 * gui_height / 4 + buttonHeight / 2, buttonWidth, buttonHeight, 1.2, true);
    }
    else
    {
        color(renderer, 255);
        rect(renderer, buttonMargin, 3 * gui_height / 4, buttonWidth, buttonHeight, true);
    }

    // Join box
    if (onButtonJoin)
    {
        color(renderer, 255, 239, 99);
        rectCenter(renderer, gui_width - buttonMargin - buttonWidth / 2, 3 * gui_height / 4 + buttonHeight / 2, buttonWidth, buttonHeight, 1.2, true);
    }
    else
    {
        color(renderer, 255);
        rect(renderer, gui_width - buttonMargin - buttonWidth, 3 * gui_height / 4, buttonWidth, buttonHeight, true);
    }

    color(renderer, 0);

    // Host button text
    imageCenter(renderer, host_button, buttonMargin + buttonWidth / 2, buttonPosY + buttonHeight / 2);

    // Join button text
    imageCenter(renderer, join_button, gui_width - buttonMargin - buttonWidth / 2, buttonPosY + buttonHeight / 2);

    // Hint message display condition
    if (live_text.length() == 0)
    {
        imageCenter(renderer, user_name_message, gui_width / 2, gui_height / 2);
    }
    else
    {
        imageCenter(renderer, live_text_texture, gui_width / 2, gui_height / 2);
    }

}

void Gui::show_codecheck()
{
    backgroundImage(renderer, gui_bg);

    color(renderer, 255);
    // Code box
    rectCenter(renderer, gui_width / 2, gui_height * 2 / 3, textBoxWidth * 2 / 3, textBoxHeight * 3 / 2, 1, true);

    color(renderer, 0);

    imageCenter(renderer, pass_code_texture, gui_width / 2, gui_height * 2 / 3);

}

void Gui::show_teamselect()
{
    backgroundImage(renderer, gui_bg);

    color(renderer, 255);

    
    // Centre Box
    imageCenter(renderer, teams[currTeam], NULL,  gui_width / 2, gui_height / 2, centreBoxLength, centreBoxLength);
    
    color (renderer, 0, 200);

    // Left Box
    imageCenter(renderer, teams[(totalTeams + currTeam - 1)%totalTeams], NULL,  gui_width / 4, gui_height / 2, sideBoxLength, sideBoxLength);
    rectCenter(renderer, gui_width / 4, gui_height / 2, sideBoxLength, sideBoxLength, 1, true);

    // Right Box
    imageCenter(renderer, teams[(totalTeams + currTeam + 1)%totalTeams], NULL,  3 * gui_width / 4, gui_height / 2, sideBoxLength, sideBoxLength);
    rectCenter(renderer, 3 * gui_width / 4, gui_height / 2, sideBoxLength, sideBoxLength, 1, true);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // Team Details

    for (int i = 0; i<4; i++)
    {
        imageCenter (renderer, player_names[i], gui_width / 2, gui_height / 2 + centreBoxLength / 2 + i * text_size + text_size);
    }
    color (renderer, 255);
    
}

void Gui::show_randomplace()
{
}

void Gui::event_username(SDL_Event event, int &state)
{
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_BACKSPACE)
    {
        if (live_text.length() > 0)
        {
            live_text = live_text.substr(0, live_text.length() - 1);
            SDL_DestroyTexture(live_text_texture);
            live_text_texture = text(renderer, live_text, live_text_font);
        }
        return;
    }
    else if (event.type == SDL_TEXTINPUT)
    {
        if (live_text.length() < maxUserNameLength && std::string(event.text.text) != " ")
        {
            live_text += event.text.text;
            SDL_DestroyTexture(live_text_texture);
            live_text_texture = text(renderer, live_text, live_text_font);
        }
        return;
    }

    // Mouse work

    int x = event.button.x;
    int y = event.button.y;

    if (event.type == SDL_MOUSEBUTTONUP)
    {

        if (inRect(x, y, buttonMargin, buttonPosY, buttonWidth, buttonHeight) && live_text.length() > 0)
        {
            isHost = true;
            setInfo();
            state++;
        }

        if (inRect(x, y, gui_width - buttonMargin - buttonWidth, buttonPosY, buttonWidth, buttonHeight) && live_text.length() > 0)
        {
            isHost = false;
            setInfo();
            state++;
        }
        
    }

    if (event.type == SDL_MOUSEMOTION)
    {

        onButtonHost = inRect(x, y, buttonMargin, buttonPosY, buttonWidth, buttonHeight);

        onButtonJoin = inRect(x, y, gui_width - buttonMargin - buttonWidth, buttonPosY, buttonWidth, buttonHeight);
    }

}

void Gui::event_codecheck(SDL_Event event, int &state)
{

    if (connected)
    {
        state++;
        return;
    }

    if (isHost)
    {
        return;
    }

    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_BACKSPACE)
    {
        if (passCode.length() > 0)
        {
            passCode = passCode.substr(0, passCode.length() - 1);
            SDL_DestroyTexture(pass_code_texture);
            pass_code_texture = text(renderer, passCode, live_text_font);
        }
        return;
    }
    else if (event.type == SDL_TEXTINPUT)
    {
        if (passCode.length() < maxUserNameLength && std::string(event.text.text) != " ")
        {
            passCode += event.text.text;
            SDL_DestroyTexture(pass_code_texture);
            pass_code_texture = text(renderer, passCode, live_text_font);
        }
        return;
    }
}

void Gui::event_teamselect(SDL_Event event, int &state)
{
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_LEFT)
    {
        for (int i = 0; i<4; i++)
        {
            SDL_DestroyTexture(player_names[i]);
        }
        currTeam = (totalTeams + currTeam + 1)%totalTeams;

        player_names[0] = text(renderer, players[currTeam][0], text_size);
        for (int i = 1; i<4; i++) 
        {
            player_names[i] = text(renderer, players[currTeam][i], text_size / 2);
        }

    }
    else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RIGHT)
    {
        for (int i = 0; i<4; i++)
        {
            SDL_DestroyTexture(player_names[i]);
        }
        currTeam = (totalTeams + currTeam - 1)%totalTeams;
        
        player_names[0] = text(renderer, players[currTeam][0], text_size);
        for (int i = 1; i<4; i++) 
        {
            player_names[i] = text(renderer, players[currTeam][i], text_size / 2);
        }
    } 
    else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN)
    {
        if(isHost) teamNum = currTeam;
        else oppenentNum = currTeam;
        state++;
    }  
}

void Gui::event_randomplace(SDL_Event event, int &state)
{
}

void Gui::setInfo()
{
    user_name = live_text;

    passCode = "";

    if (isHost)
    {
        srand(time(0));

        for (int i = 0; i < 6; i++)
        {
            passCode += (char) (rand() % 26 + (int) 'A'); 
        }
    }

    color(renderer, 0);
    pass_code_texture = text(renderer, passCode, live_text_font);

    connectionThread = SDL_CreateThread(initialConnection , "Connect", this);

}

int Gui::initialConnection(void* a)
{
    // arg = "1"; Host
    // arg = "0"; Join

    Gui* gui = (Gui*) a;

    gui->connected = false;

    if (gui->isHost)
    {
        serv = new Server("127.0.0.1");

        std::string user_code;

        do
        {
            user_code = serv->get();
            if (user_code == gui->passCode)
            {
                serv->send("1");
                serv->send(user_name);
                oponent_name = serv->get();
                break;
            }
            else
            {
                serv->send("0");
            }
        } while (!gui->abort);
    }
    else
    {
        client = new Client("127.0.0.1");

        do
        {
            client->send(gui->passCode);

            std::string response = client->get();

            if (response == "1")
            {
                oponent_name = client->get();
                client->send(user_name);
                break;
            }
        } while (!gui->abort);
    }

    if (!gui->abort)
    {
        std::cout << "User name: " << user_name << std::endl;
        std::cout << "Oponenet name: " << oponent_name << std::endl;
    }

    gui->connected = !gui->abort;

    return 0;
}

void Gui::reel(bool dir)
{
    float dx = gui_width / 4;
    float dw = (centreBoxLength - sideBoxLength) / 2;

    int frames = 100;
    dx /= frames;
    dw /= frames;

    int count = 0;
    while(count != frames)
    {
        // Centre Box
        imageCenter(renderer, teams[currTeam], NULL,  gui_width / 2, gui_height / 2, centreBoxLength, centreBoxLength);
        
        color (renderer, 0, 134);

        // Left Box
        imageCenter(renderer, teams[(totalTeams + currTeam - 1)%totalTeams], NULL,  gui_width / 4, gui_height / 2, sideBoxLength, sideBoxLength);
        rectCenter(renderer, gui_width / 4, gui_height / 2, sideBoxLength, sideBoxLength, 1, true);

        // Right Box
        imageCenter(renderer, teams[(totalTeams + currTeam + 1)%totalTeams], NULL,  3 * gui_width / 4, gui_height / 2, sideBoxLength, sideBoxLength);
        rectCenter(renderer, 3 * gui_width / 4, gui_height / 2, sideBoxLength, sideBoxLength, 1, true);

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);   
    }
}
