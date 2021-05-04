#include "teamView.hpp"

Teamview::Teamview()
{

}

Teamview::Teamview(SDL_Renderer* rend, Maze* maze, int teamN, bool self)
{
    std::cout<<"FF"<<std::endl;
    renderer = rend;
    std::cout << "renderer" << std::endl;
    game_maze = maze;
    std::cout << "maze" << std::endl;
    team = teamN;
    std::cout << "teamN" << std::endl;
    isMyTeam = self;

    std::cout << "in" << std::endl;

    std::vector<Character *> v[4];

    v[0].push_back(new Character(team, 0, self));

    for (int i = 0; i < 4; i++) 
    {
        v[1].push_back(new Character(team, 1, self));
    }

    for (int i = 0; i < 2; i++) 
    {
        v[2].push_back(new Character(team, 2, self));
    }

    v[3].push_back(new Character(team, 3, self));

    for (int i = 0; i < 4; i++)
    {
        characters.push_back(v[i]);
    }
    
    std::cout<<"out"<<std::endl;

}

void Teamview::show()
{
    color(renderer, 0, 160);
    rectCenter(renderer, block_ox + block_width / 2, block_oy + block_height / 2, block_width, block_height, 0.95, true);
    
    color(renderer, 197, 207, 68, 160);

    for (int i = 0; i<4; i++)
    {
        rectCenter(renderer, block_ox + tile_height / 2, block_oy + tile_height * i  +  tile_height / 2, tile_height, tile_height, 0.9, true);
        imageCenter(renderer, characters[i][0]->spriteSheet, &src, block_ox +tile_height / 2, block_oy + tile_height * i  +  tile_height / 2, tile_height, tile_height, 0.95);
    }

    for (int i = 0; i<4; i++)
    {
        for (int j = 0; j<4; j++)
        {

            if (j == 0) bar_fill = bar_width * characters[i][0]->prop.power / max[j];
            else if (j == 1) bar_fill = bar_width * characters[i][0]->prop.health / max[j];
            else if (j == 2) bar_fill = bar_width * characters[i][0]->prop.range / max[j];
            else if (j == 3) bar_fill = bar_width * characters[i][0]->prop.speed / max[j];
            else ;

            color(renderer, 0, 255);
            rect(renderer, block_ox + tile_height, block_oy + i * tile_height + j * bar_height + bar_height / 4, bar_width, bar_height / 4, true);
            color(renderer, color_comb[j][0], color_comb[j][1], color_comb[j][2], 255);
            rect(renderer, block_ox + tile_height, block_oy + i * tile_height + j * bar_height + bar_height / 4, bar_fill, bar_height / 4, true);
        }
    }
}