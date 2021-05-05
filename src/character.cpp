#include "character.hpp"

SDL_Rect Character::spriteDim[4][3] = 
{
    {{.x = 0, .y = 0, .w = 48, .h = 49}, {.x = 48, .y = 0, .w = 48, .h = 49}, {.x = 96, .y = 0, .w = 48, .h = 49}},
    {{.x = 0, .y = 49, .w = 48, .h = 49}, {.x = 48, .y = 49, .w = 48, .h = 49}, {.x = 96, .y = 49, .w = 48, .h = 49}},
    {{.x = 0, .y = 98, .w = 48, .h = 49}, {.x = 48, .y = 98, .w = 48, .h = 49}, {.x = 96, .y = 98, .w = 48, .h = 49}},
    {{.x = 0, .y = 147, .w = 48, .h = 49}, {.x = 48, .y = 147, .w = 48, .h = 49}, {.x = 96, .y = 147, .w = 48, .h = 49}},
};

std::map<int, properties> Character::charProp = 
        {
            std::make_pair(0, properties(5, 12, 8, 2)),
            std::make_pair(1, properties(2, 5, 4, 6)),
            std::make_pair(2, properties(4, 8, 7, 4)),
            std::make_pair(3, properties(6, 10, 5, 3))
        };

Character::Character()
{

}

Character::Character(SDL_Renderer* renderer, Maze* maze, int teamN, int lev, bool t)
{
    this->renderer = renderer;
    game_maze = maze;
    team = teamN;
    level = lev;
    isMyTeam = t;
    prop = charProp[lev];
    spriteSheet = loadTexture(("./assets/images/characters/t" + std::to_string(team+1) + "l" + std::to_string(lev) + ".png").c_str(), renderer);
}

void Character::show()
{
    if (!active || dead)
    {
        return;
    }
    SDL_RenderCopyF(renderer, spriteSheet, &spriteRect, &posRect);
}

void Character::deploy(int i, int j)
{
    currPos[0] = i;
    currPos[1] = j;

    vel[0] = vel[1] = 0;

    active = true;

    spriteRect = spriteDim[0][1];

    posRect = {.x = currPos[1] * game_maze->cell_size + game_maze->cell_size / 2,
               .y = currPos[0] * game_maze->cell_size + game_maze->cell_size / 2,
               .w = game_maze->cell_size * (float) 0.95,
               .h = game_maze->cell_size * (float) 0.95 };
}

void Character::update()
{
    if (!active || dead)
    {
        return;
    }

    // Sprite update
    if (vel[0] > 0)
    {
        spriteRect = spriteDim[2][currSprite];
        currSprite = (currSprite + 1)%3;
    }
    else if (vel[0] < 0) 
    {
        spriteRect = spriteDim[1][currSprite];
        currSprite = (currSprite + 1)%3;
    }
    else if (vel[1] > 0)
    {
        spriteRect = spriteDim[0][currSprite];
        currSprite = (currSprite + 1)%3;
    }
    else if (vel[1] < 0)
    {
        spriteRect = spriteDim[3][currSprite];
        currSprite = (currSprite + 1)%3;
    }


    //Pos Update


}
