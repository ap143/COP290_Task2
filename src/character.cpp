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
            std::make_pair(0, properties(5, 2400, 4, 2)),
            std::make_pair(1, properties(2, 1000, 2, 6)),
            std::make_pair(2, properties(4, 1600, 3, 4)),
            std::make_pair(3, properties(6, 2000, 4, 3))
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
    health = prop.health;
    spriteSheet = loadTexture(("./assets/images/characters/t" + std::to_string(team+1) + "l" + std::to_string(lev) + ".png").c_str(), renderer);
}

void Character::show()
{
    if (!active)
    {
        return;
    }
    imageCenter(renderer, spriteSheet, &spriteRect, posRect.x, posRect.y, posRect.w, posRect.h);

    color(renderer, 255 * (1 - health / prop.health), 255 * health / prop.health, 0);
    rect(renderer, posRect.x + posRect.w / 2, posRect.y - posRect.h / 2, (float) posRect.w * health / prop.health, (float) posRect.h / 16, true);
}

void Character::deploy(int i, int j)
{
    currPos[0] = i;
    currPos[1] = j;

    vel[0] = vel[1] = 0;
    currDir = isMyTeam ? 3 : 1;

    active = true;
    ready = true;

    spriteRect = currDir == 3 ? spriteDim[1][1] : spriteDim[2][1];

    posRect = {.x = game_maze->ox + currPos[1] * game_maze->cell_size + game_maze->cell_size / 2,
               .y = game_maze->oy + currPos[0] * game_maze->cell_size + game_maze->cell_size / 2,
               .w = game_maze->cell_size * (float) 0.8,
               .h = game_maze->cell_size * (float) 0.8 };
}

void Character::update()
{
    if (!active)
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
    dx += vel[0] * velConst;
    dy += vel[1] * velConst;

    posRect.x += (vel[0] * velConst) * game_maze->cell_size;
    posRect.y += (vel[1] * velConst) * game_maze->cell_size;


    if (dx >= 1 || dy >= 1 || dx <= -1 || dy <= -1)
    {
        if (dx >= 1) currPos[1] += 1;
        else if (dx <= -1) currPos[1] -= 1;
        else if (dy >= 1) currPos[0] += 1;
        else if (dy <= -1) currPos[0] -= 1;
        else ;

        //decide next cell to move
        dx = dy = 0;
        vel[0] = vel[1] = 0;
        ready = true;
    }

}

void Character::setVel(int dir)
{
    if (!active)
    {
        return;
    }

    turn(dir);

    if (!game_maze->maze[currPos[0]][currPos[1]][dir])
    {
        return;
    }

    switch (dir)
    {
        case 0: vel[1] = -1; break;
        case 1: vel[0] = 1; break;
        case 2: vel[1] = 1; break;
        case 3: vel[0] = -1; break;
        default: break;
    }

    ready = false;
}

void Character::turn(int dir)
{
    if (!active)
    {
        return;
    }

    if (vel[0] == 1)
    {
        currPos[1] += 1;
    }
    else if (vel[0] == -1)
    {
        currPos[1] -= 1;
    }
    else if (vel[1] == 1)
    {
        currPos[0] += 1;
    }
    else if (vel[1] == -1)
    {
        currPos[0] -= 1;
    }

    dx = dy = 0;
    vel[0] = vel[1] = 0;
    ready = true;

    if (dir > 3 || dir < 0)
    {
        return;
    }

    currDir = dir;
    switch (dir)
    {
    case 0:
        spriteRect = spriteDim[3][1];
        break;
    case 1:
        spriteRect = spriteDim[2][1];
        break;
    case 2:
        spriteRect = spriteDim[0][1];
        break;
    case 3:
        spriteRect = spriteDim[1][1];
        break;
    default:
        break;
    }
}

void Character::attack(int power)
{
    health -= power;
    if (health <= 0)
    {
        active = false;
        dead = true;
        if (level == 0)
        {
            game_over = true;
        }
    }
}
