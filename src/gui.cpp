#include "gui.hpp"

Gui::Gui(SDL_Window *window, SDL_Renderer *renderer)
{
    this->window = window;
    this->renderer = renderer;

    gui_bg = loadTexture("./assets/images/bg.png", renderer);

    live_text = "";

    font1 = TTF_OpenFont("./assets/fonts/CONSOLAB.TTF", live_text_font);
    font2 = TTF_OpenFont("./assets/fonts/CONSOLAB.TTF", 2 * live_text_font / 3);
    font3 = TTF_OpenFont("./assets/fonts/CONSOLAB.TTF", live_text_font / 2);

    // Solid Black texts
    color(renderer, 0);
    live_text_texture = text(renderer, live_text, font1);
    host_button = text(renderer, "Host", font1);
    join_button = text(renderer, "Join", font1);
    pass_code_texture = text(renderer, "", font1);

    // Grey texts
    color(renderer, 150);
    user_name_message = text(renderer, "Username...", font2);
    waiting_texture = text(renderer, "Waiting...", font1);
    input_code = text(renderer, "Code...", font1);

    // Button hover bools
    onButtonHost = onButtonJoin = false;

    // Team sprite inputs
    for (int i = 0; i < 10; i++)
    {
        teams.push_back(loadTexture(("./assets/images/characters/t" + std::to_string(i + 1) + ".png").c_str(), renderer));
    }

    color(renderer, 255);
    for (int i = 0; i < 4; i++)
    {
        player_names.push_back(text(renderer, players[currTeam][i], font2));
        player_counts.push_back(text(renderer, num_players[i], font3));
    }

    power = text(renderer, "Power", font2);
    health = text(renderer, "Health", font2);
    speed = text(renderer, "Speed", font2);
    heal = text(renderer, "Heal", font2);

    std::ifstream data("./assets/character_data.csv");

    data >> Character::total_teams;
    int n = Character::total_teams;

    int pi, hi, si, hli;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < n; j++)
        {
            data >> pi >> hi >> si >> hli;

            Character::character_data.push_back(properties(pi, hi, si, hli));
        }
    }

    data.close();

    selected = loadTexture("./assets/images/selected.png", renderer);
}

Gui::~Gui()
{
    TTF_CloseFont(font1);
    TTF_CloseFont(font2);
    TTF_CloseFont(font3);

    SDL_DestroyTexture(gui_bg);

    SDL_DestroyTexture(live_text_texture);
    SDL_DestroyTexture(host_button);
    SDL_DestroyTexture(join_button);
    SDL_DestroyTexture(pass_code_texture);    
    
    SDL_DestroyTexture(user_name_message);
    SDL_DestroyTexture(waiting_texture);
    SDL_DestroyTexture(input_code);

    SDL_DestroyTexture(selected);

    SDL_DestroyTexture(power);
    SDL_DestroyTexture(health);
    SDL_DestroyTexture(speed);
    SDL_DestroyTexture(heal);

    for (SDL_Texture *tex : teams)
    {
        if (tex != NULL)
        {
            SDL_DestroyTexture(tex);
        }
    }

    for (SDL_Texture *tex : player_names)
    {
        if(tex != NULL)
        {
            SDL_DestroyTexture(tex);
        }
    }

    for (SDL_Texture *tex : player_counts)
    {
        if (tex != NULL)
        {
            SDL_DestroyTexture(tex);
        }
    }

    if (serv != nullptr)
    {
        serv->end();
    }

    if (client != nullptr)
    {
        client->end();
    }

    if (connectionThread != nullptr)
    {
        SDL_DetachThread(connectionThread);
    }

    if (runnerThread != nullptr)
    {
        SDL_DetachThread(runnerThread);
    }

    if (serv != nullptr)
    {
        delete serv;
    }
    if (client != nullptr)
    {
        delete client;
    }
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
        rectCenter(renderer, buttonMargin + buttonWidth / 2, buttonPosY + buttonHeight / 2, buttonWidth, buttonHeight, 1.2, true);
    }
    else
    {
        color(renderer, 255);
        rect(renderer, buttonMargin, buttonPosY, buttonWidth, buttonHeight, true);
    }

    // Join box
    if (onButtonJoin)
    {
        color(renderer, 255, 239, 99);
        rectCenter(renderer, gui_width - buttonMargin - buttonWidth / 2, buttonPosY + buttonHeight / 2, buttonWidth, buttonHeight, 1.2, true);
    }
    else
    {
        color(renderer, 255);
        rect(renderer, gui_width - buttonMargin - buttonWidth, buttonPosY, buttonWidth, buttonHeight, true);
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
    rectCenter(renderer, gui_width / 2, gui_height / 2, textBoxWidth * 2 / 3, textBoxHeight * 3 / 2, 1, true);

    color(renderer, 0);

    if (passCode.length() > 0)
    {
        imageCenter(renderer, pass_code_texture, gui_width / 2, gui_height / 2);
    }
    else
    {
        imageCenter(renderer, input_code, gui_width / 2, gui_height / 2);
    }
}

void Gui::team_select_update()
{
    animate_extent += animate_direction;

    if (animate_extent == max_extent)
    {
        for (int i = 0; i < 4; i++)
        {
            if(player_names[i] != NULL)
            {
                SDL_DestroyTexture(player_names[i]);
            }
        }
        currTeam = (totalTeams + currTeam - 1) % totalTeams;

        for (int i = 0; i < 4; i++)
        {
            player_names[i] = text(renderer, players[currTeam][i], font2);
        }
        animate_extent = animate_direction = 0;
    }
    else if (animate_extent == -max_extent)
    {
        for (int i = 0; i < 4; i++)
        {
            if(player_names[i] != NULL)
            {
                SDL_DestroyTexture(player_names[i]);
            }
        }
        currTeam = (totalTeams + currTeam + 1) % totalTeams;

        for (int i = 0; i < 4; i++)
        {
            player_names[i] = text(renderer, players[currTeam][i], font2);
        }
        animate_extent = animate_direction = 0;
    }
}

void Gui::show_teamselect()
{
    team_select_update();

    backgroundImage(renderer, gui_bg);

    float scl = std::abs(animate_extent) / max_extent;
    float cx;
    float cy = gui_height / 2;
    float len;

    // New Box

    if (animate_extent < 0)
    {
        color(renderer, 0, 150);
        cx = 3 * gui_width / 4;
        len = sideBoxLength * scl;
        imageCenter(renderer, teams[(totalTeams + currTeam + 2) % totalTeams], NULL, cx, cy, len, len);
        if ((totalTeams + currTeam + 2) % totalTeams == opponentTeamNum)
        {
            imageCenter(renderer, selected, NULL, cx, cy, len, len);
        }
        rectCenter(renderer, cx, cy, len, len, 1, true);
    }
    else if (animate_extent > 0)
    {
        color(renderer, 0, 150);
        cx = gui_width / 4;
        len = sideBoxLength * scl;
        imageCenter(renderer, teams[(totalTeams + currTeam - 2) % totalTeams], NULL, cx, cy, len, len);
        if ((totalTeams + currTeam - 2) % totalTeams == opponentTeamNum)
        {
            imageCenter(renderer, selected, NULL, cx, cy, len, len);
        }
        rectCenter(renderer, cx, cy, len, len, 1, true);
    }

    // Left Box
    if (animate_extent < 0)
    {
        color(renderer, 0, 150);
        cx = gui_width / 4;
        len = sideBoxLength * (1 - scl);
    }
    else
    {
        color(renderer, 0, 150 * (1 - scl));
        cx = gui_width / 4 * (1 + scl);
        len = sideBoxLength * (1 + scl);
    }

    imageCenter(renderer, teams[(totalTeams + currTeam - 1) % totalTeams], NULL, cx, cy, len, len);
    if ((totalTeams + currTeam - 1) % totalTeams == opponentTeamNum)
    {
        color(renderer, 0, 150);
        imageCenter(renderer, selected, NULL, cx, cy, len, len);
    }
    rectCenter(renderer, cx, cy, len, len, 1, true);

    // Right Box
    if (animate_extent > 0)
    {
        color(renderer, 0, 150);
        cx = 3 * gui_width / 4;
        len = sideBoxLength * (1 - scl);
    }
    else
    {
        color(renderer, 0, 150 * (1 - scl));
        cx = gui_width / 2 + gui_width / 4 * (1 - scl);
        len = sideBoxLength * (1 + scl);
    }

    imageCenter(renderer, teams[(totalTeams + currTeam + 1) % totalTeams], NULL, cx, cy, len, len);
    if ((totalTeams + currTeam + 1) % totalTeams == opponentTeamNum)
    {
        color(renderer, 0, 150);
        imageCenter(renderer, selected, NULL, cx, cy, len, len);
    }
    rectCenter(renderer, cx, cy, len, len, 1, true);

    // Centre Box
    cx = gui_width / 2 + animate_extent * gui_width / 4 / max_extent;
    len = centreBoxLength * (1 - scl / 2);

    imageCenter(renderer, teams[currTeam], NULL, cx, cy, len, len);
    if (currTeam == opponentTeamNum)
    {
        imageCenter(renderer, selected, NULL, cx, cy, len, len);
        color(renderer, 0, 150);
        rectCenter(renderer, cx, cy, len, len, 1, true);
    }
    else
    {
        color(renderer, 0, 150 * scl);
        rectCenter(renderer, cx, cy, len, len, 1, true);
    }

    // Team Details

    float ox = gui_width / 2 - 2 * bar_length / 3;
    float oy = gui_height / 2 + 0.7 * centreBoxLength;

    color(renderer, 255, 255, 255, 255);
    image(renderer, power, ox, oy, 0, SDL_FLIP_NONE);
    image(renderer, health, ox + bar_length + bar_length / 10, oy, 0, SDL_FLIP_NONE);
    image(renderer, speed, ox + 2 * bar_length + 2 * bar_length / 10, oy, 0, SDL_FLIP_NONE);
    image(renderer, heal, ox + 3 * bar_length + 3 * bar_length / 10, oy, 0, SDL_FLIP_NONE);

    for (int i = 0; i < 4; i++)
    {
        color(renderer, 255, 255, 255, 255);
        image(renderer, player_names[i], ox - 4 * bar_length, oy + i * text_size + text_size, 0, SDL_FLIP_NONE);

        image(renderer, player_counts[i], ox - bar_length / 2, oy + i * text_size + text_size, 0, SDL_FLIP_NONE);

        color(renderer, 255, 0, 0, 255);
        rect(renderer, ox + 0 * bar_length / 10, oy + i * text_size + text_size, bar_length * Character::character_data[i * Character::total_teams + currTeam].power / max_power, bar_width, true);

        color(renderer, 0, 255, 0, 255);
        rect(renderer, ox + bar_length + 1 * bar_length / 10, oy + i * text_size + text_size, bar_length * Character::character_data[i * Character::total_teams + currTeam].health / max_health, bar_width, true);

        color(renderer, 0, 0, 255, 255);
        rect(renderer, ox + 2 * bar_length + 2 * bar_length / 10, oy + i * text_size + text_size, bar_length * Character::character_data[i * Character::total_teams + currTeam].speed / max_speed, bar_width, true);

        color(renderer, 252, 223, 3, 255);
        rect(renderer, ox + 3 * bar_length + 3 * bar_length / 10, oy + i * text_size + text_size, bar_length * Character::character_data[i * Character::total_teams + currTeam].heal / max_heal, bar_width, true);
    }
    color(renderer, 255);
}

void Gui::show_randomplace()
{
    std::cout << teamNum << " " << opponentTeamNum << std::endl;
}

void Gui::event_username(SDL_Event event, int &state)
{
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_BACKSPACE)
    {
        if (live_text.length() > 0)
        {
            live_text = live_text.substr(0, live_text.length() - 1);
            if (live_text_texture != NULL)
            {
                SDL_DestroyTexture(live_text_texture);
            }
            live_text_texture = text(renderer, live_text, font1);
        }
        return;
    }
    else if (event.type == SDL_TEXTINPUT)
    {
        if (live_text.length() < maxUserNameLength && std::string(event.text.text) != " ")
        {
            live_text += event.text.text;
            if (live_text_texture != NULL)
            {
                SDL_DestroyTexture(live_text_texture);
            }
            live_text_texture = text(renderer, live_text, font1);
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
            if (pass_code_texture != NULL)
            {
                SDL_DestroyTexture(pass_code_texture);
            }
            pass_code_texture = text(renderer, passCode, font1);
        }
        return;
    }
    else if (event.type == SDL_TEXTINPUT)
    {
        if (passCode.length() < maxUserNameLength && std::string(event.text.text) != " ")
        {
            passCode += event.text.text;
            if (pass_code_texture != NULL)
            {
                SDL_DestroyTexture(pass_code_texture);
            }
            pass_code_texture = text(renderer, passCode, font1);
        }
        return;
    }
}

void Gui::event_teamselect(SDL_Event event, int &state)
{
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_LEFT)
    {
        animate_direction = -1;
    }
    else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RIGHT)
    {
        animate_direction = 1;
    }
    else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN)
    {
        if (animate_direction != 0)
        {
            return;
        }
        if (currTeam != opponentTeamNum)
        {
            teamNum = currTeam;
            sendMessage(TEAM_SELECT + std::to_string(teamNum));
            state += 2;
        }
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
            passCode += (char)(rand() % 26 + (int)'A');
        }
    }

    color(renderer, 0);
    pass_code_texture = text(renderer, passCode, font1);

    connectionThread = SDL_CreateThread(initialConnection, "Connect", this);
}

int Gui::initialConnection(void *a)
{
    Gui *gui = (Gui *)a;

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
                serv->send("connection_success");
                serv->send(user_name);
                opponent_name = serv->get();
                break;
            }
            else
            {
                serv->send("connection_error");
            }
        } while (serv->isActive());
    }
    else
    {
        client = new Client("127.0.0.1");

        do
        {
            client->send(gui->passCode);

            std::string response = client->get();

            if (response == "connection_success")
            {
                opponent_name = client->get();
                client->send(user_name);
                break;
            }
        } while (client->isActive());
    }

    if (!gui->abort)
    {
        std::cout << "User name: " << user_name << std::endl;
        std::cout << "Oponenet name: " << opponent_name << std::endl;
    }

    gui->connected = !gui->abort;

    gui->runnerThread = SDL_CreateThread(runGame, "Connect", gui);

    return 0;
}

int Gui::runGame(void *a)
{
    Gui *gui = (Gui *)a;

    std::string response;

    if (gui->isHost)
    {
        while (serv->isActive())
        {
            response = serv->get();
            respond(response);
        }
    }
    else
    {
        while (client->isActive())
        {
            response = client->get();
            respond(response);
        }
    }

    return 0;
}
