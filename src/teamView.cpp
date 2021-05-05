#include "teamView.hpp"

Teamview::Teamview(SDL_Renderer* rend, Maze* maze, int teamN, bool self)
{
    renderer = rend;
    game_maze = maze;
    team = teamN;
    isMyTeam = self;

    deployRange = game_maze->n * 2 / 5;

    std::vector<Character *> v[4];

    v[0].push_back(new Character(renderer, maze, team, 0, self));

    for (int i = 0; i < 4; i++)
    {
        v[1].push_back(new Character(renderer, maze, team, 1, self));
    }

    for (int i = 0; i < 2; i++) 
    {
        v[2].push_back(new Character(renderer, maze, team, 2, self));
    }

    v[3].push_back(new Character(renderer, maze, team, 3, self));

    for (int i = 0; i < 4; i++)
    {
        characters.push_back(v[i]);
    }

    block_ox = game_maze->ox + game_maze->grid_length;
    block_oy = gui_height / 2;
    block_width = gui_width - block_ox;
    block_height = gui_height - block_oy;

    src = {.x = 48, .y = 0, .w = 48, .h = 49};

    tile_height = block_height / 4;
    bar_height = tile_height / 4;
    bar_width = (block_width - tile_height) * 0.9;
    bar_fill = 0;


    for (int i = 0; i < 4; i++)
    {
        tiles[i] = {.x = block_ox, .y = block_oy + i * tile_height, .w = tile_height, .h = tile_height};

        count_text[i] = text(renderer, std::to_string(count[i]), count_text_size);
    }

}

void Teamview::show()
{
    if (isMyTeam)
    {
        if(activeLevel == 0 && !kingDeployed)
        {
            color(renderer, 0, 255, 0, 100);
            rect(renderer, game_maze->ox + (game_maze->n - deployRange) * game_maze->cell_size, game_maze->oy + (game_maze->n - deployRange) * game_maze->cell_size, deployRange * game_maze->cell_size, deployRange * game_maze->cell_size, true);
        }

        color(renderer, 0, 160);
        rectCenter(renderer, block_ox + block_width / 2, block_oy + block_height / 2, block_width, block_height, 0.95, true);
        
        for (int i = 0; i<4; i++)
        {
            if (activeLevel == i && count[i] > 0)
            {
                color(renderer, 197, 255, 68, 160);
            }
            else
            {
                color(renderer, 197, 207, 68, 160);   
            }
            
            // Tile on which sprite placed
            rectCenter(renderer, block_ox + tile_height / 2, block_oy + tile_height * i  +  tile_height / 2, tile_height, tile_height, 0.95, true);

            // Sprite 
            imageCenter(renderer, characters[i][0]->spriteSheet, &src, block_ox + tile_height / 2, block_oy + tile_height * i  + tile_height / 2, tile_height, tile_height, 0.9);

            // Text
            image(renderer, count_text[i], tiles[i].x, tiles[i].y, 0, SDL_FLIP_NONE);


            if (count[i] == 0)
            {
                // Shade when count of a level = 0
                color(renderer, 0, 160);
                rectCenter(renderer, block_ox + tile_height / 2, block_oy + tile_height * i  +  tile_height / 2, tile_height, tile_height, 0.95, true);
            }
        }
    }

    for (std::vector<Character *> v: characters)
    {
        for (Character *c: v)
        {
            c->show();
        }
    }
}

void Teamview::handleEvent(SDL_Event event)
{

    if (event.type == SDL_KEYDOWN && opponentKingDeployed && kingDeployed)
    {
        // Move king
        Character *king = characters[0][0];
        if (!king->ready)
        {
            return;
        }

        int dir = -1;
        switch (event.key.keysym.sym)
        {
            case SDLK_UP: king->setVel(dir = 0); break;
            case SDLK_DOWN: king->setVel(dir = 2); break;
            case SDLK_RIGHT: king->setVel(dir = 1); break;
            case SDLK_LEFT: king->setVel(dir = 3); break;
            default: break;
        }

        if (std::abs(king->vel[0]) + std::abs(king->vel[1]) == 0)
        {
            dir = -1;
        }

        if (dir >= 0)
        {
            sendMessage(MOVEMENT + std::string("00") + std::to_string(dir));
        }
        else
        {
            sendMessage(TURN + std::string("00") + std::to_string(king->currDir));
        }

        return;
    }

    if (event.type != SDL_MOUSEBUTTONUP)
    {
        return;
    }

    float x = event.button.x;
    float y = event.button.y;
    for (int i = 0; i < 4; i++)
    {    
        if (inRect(x, y, tiles[i].x, tiles[i].y, tiles[i].w, tiles[i].h) && count[i] > 0)
        {
            if ((!opponentKingDeployed || !kingDeployed ) && i > 0)
            {
                continue;
            }
            activeLevel = i;
            return;
        }
    }

    if (activeLevel == -1)
    {
        return;
    }

    if (count[activeLevel] <= 0)
    {
        activeLevel = -1;
        return;
    }

    if (inRect(x, y, game_maze->ox, game_maze->oy, game_maze->grid_length, game_maze->grid_length))
    {
        x -= game_maze->ox;
        y -= game_maze->oy;

        int j = x / game_maze->cell_size;
        int i = y / game_maze->cell_size;

        if (!kingDeployed)
        {
            if ((game_maze->n - i > deployRange) || (game_maze->n - j > deployRange))
            {
                return;
            }
        }

        characters[activeLevel][count[activeLevel]-1]->deploy(i, j);
        count[activeLevel]--;

        sendMessage(DEPLOY + std::to_string(activeLevel) + std::to_string(count[activeLevel]) + ((i < 10) ? "0" : "") + std::to_string(i) + ((j < 10) ? "0" : "") + std::to_string(j));

        kingDeployed = true;
    } 

}

void Teamview::update()
{
    for (std::vector<Character *> v: characters)
    {
        for (Character *c: v)
        {
            if (c->ready && isMyTeam)
            {
                setNextDest(c);
            }
            c->update();
        }
    }
}

void Teamview::deploy(int level, int cnt, int i, int j)
{
    i = game_maze->n - i - 1;
    j = game_maze->n - j - 1;

    characters[level][cnt]->deploy(i, j);
    count[level]--;
}

void Teamview::setNextDest(Character* c)
{

}
