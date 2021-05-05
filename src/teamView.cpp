#include "teamView.hpp"

Teamview::Teamview(SDL_Renderer* rend, Maze* maze, int teamN, bool self)
{
    renderer = rend;
    game_maze = maze;
    team = teamN;
    isMyTeam = self;

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
        color(renderer, 0, 160);
        rectCenter(renderer, block_ox + block_width / 2, block_oy + block_height / 2, block_width, block_height, 0.95, true);
        
        color(renderer, 197, 207, 68, 160);

        for (int i = 0; i<4; i++)
        {
            rectCenter(renderer, block_ox + tile_height / 2, block_oy + tile_height * i  +  tile_height / 2, tile_height, tile_height, 0.9, true);
            imageCenter(renderer, characters[i][0]->spriteSheet, &src, block_ox +tile_height / 2, block_oy + tile_height * i  +  tile_height / 2, tile_height, tile_height, 0.9);
            image(renderer, count_text[i], tiles[i].x, tiles[i].y, 0, SDL_FLIP_NONE);;
        }
    }

    for (std::vector<Character *> v: characters)
    {
        for (Character *c: v)
        {
            c->show();
        }
    }

    // for (int i = 0; i<4; i++)
    // {
    //     for (int j = 0; j<4; j++)
    //     {

    //         if (j == 0) bar_fill = bar_width * charProp[i].power / max[j];
    //         else if (j == 1) bar_fill = bar_width * charProp[i].health / max[j];
    //         else if (j == 2) bar_fill = bar_width * charProp[i].range / max[j];
    //         else if (j == 3) bar_fill = bar_width * charProp[i].speed / max[j];
    //         else ;

    //         color(renderer, 0, 255);
    //         rect(renderer, block_ox + tile_height, block_oy + i * tile_height + j * bar_height + bar_height / 4, bar_width, bar_height / 4, true);
    //         color(renderer, color_comb[j][0], color_comb[j][1], color_comb[j][2], 255);
    //         rect(renderer, block_ox + tile_height, block_oy + i * tile_height + j * bar_height + bar_height / 4, bar_fill, bar_height / 4, true);
    //     }
    // }
}

void Teamview::handleEvent(SDL_Event event)
{
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

        characters[activeLevel][count[activeLevel]-1]->deploy(i, j);
        count[activeLevel]--;
    } 

}

void Teamview::update()
{
    for (std::vector<Character *> v: characters)
    {
        for (Character *c: v)
        {
            c->update();
        }
    }
}