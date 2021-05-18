#include "character.hpp"

SDL_Rect Character::spriteDim[4][3] = 
{
    {{.x = 0, .y = 0, .w = 48, .h = 49}, {.x = 48, .y = 0, .w = 48, .h = 49}, {.x = 96, .y = 0, .w = 48, .h = 49}},
    {{.x = 0, .y = 49, .w = 48, .h = 49}, {.x = 48, .y = 49, .w = 48, .h = 49}, {.x = 96, .y = 49, .w = 48, .h = 49}},
    {{.x = 0, .y = 98, .w = 48, .h = 49}, {.x = 48, .y = 98, .w = 48, .h = 49}, {.x = 96, .y = 98, .w = 48, .h = 49}},
    {{.x = 0, .y = 147, .w = 48, .h = 49}, {.x = 48, .y = 147, .w = 48, .h = 49}, {.x = 96, .y = 147, .w = 48, .h = 49}},
};

std::vector<properties> Character::character_data = {};

int Character::total_teams = 0;

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
    prop = Character::character_data[level * Character::total_teams + team];
    health = prop.health;

    spriteSheet = loadTexture(("./assets/images/characters/t" + std::to_string(team+1) + "l" + std::to_string(lev) + ".png").c_str(), renderer);
    attackSheet = loadTexture(("./assets/images/characters/t" + std::to_string(team+1) + "l" + std::to_string(lev) +  "_blast.png").c_str(), renderer);

    SDL_QueryTexture(attackSheet, NULL, NULL, &w, &h);
    blastRect = {.x = 0, .y = 0, w / 6, h};

    if (level == 0)
    {
        prop.power *= 5;
    }
}

Character::~Character()
{
    if (spriteSheet != NULL)
    {
        SDL_DestroyTexture(spriteSheet);
    }
}

void Character::show()
{
    if (!active)
    {
        return;
    }

    // Sprite 
    imageCenter(renderer, spriteSheet, &spriteRect, posRect.x, posRect.y, posRect.w, posRect.h);

    if (attackDir == 0)
    {
        imageCenter(renderer, attackSheet, &blastRect, posRect.x, posRect.y - game_maze->cell_size / 3, posRect.w / 2, posRect.w / 2);
    }
    else if (attackDir == 1)
    {
        imageCenter(renderer, attackSheet, &blastRect, posRect.x + game_maze->cell_size / 3, posRect.y, posRect.w / 2, posRect.w / 2);
    }
    else if (attackDir == 2)
    {
        imageCenter(renderer, attackSheet, &blastRect, posRect.x, posRect.y + game_maze->cell_size / 3, posRect.w / 2, posRect.w / 2);   
    }
    else if (attackDir == 3)
    {
        imageCenter(renderer, attackSheet, &blastRect, posRect.x - game_maze->cell_size / 3, posRect.y, posRect.w / 2, posRect.w / 2);   
    }
    else if (attackDir == 4)
    {
        imageCenter(renderer, attackSheet, &blastRect, posRect.x, posRect.y, posRect.w / 2, posRect.w / 2);
    }

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

    if (sprite_change == max_speed + 1 - prop.speed)
    {
        sprite_change = 0;
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
    }
    else
    {
        sprite_change++;
    }

    if(attackDir != -1)
    {
        blastRect.x = currBlast * blastRect.w;
        currBlast = (currBlast + 1)%6;
    }

    //Pos Update
    dx += vel[0] * velConst * prop.speed;
    dy += vel[1] * velConst * prop.speed;

    posRect.x = game_maze->ox + currPos[1] * game_maze->cell_size + 
                game_maze->cell_size / 2 + dx * game_maze->cell_size;
    posRect.y = game_maze->oy + currPos[0] * game_maze->cell_size + 
                game_maze->cell_size / 2 + dy * game_maze->cell_size;

    if (!changed)
    {
        if (dx >= 0.5)
        {
            currPos[1]++;
            dx += -1;
            changed = true;
        }
        else if (dx <= -0.5)
        {
            currPos[1]--;
            dx += 1;
            changed = true;
        }
        else if (dy >= 0.5)
        {
            currPos[0]++;
            dy += -1;
            changed = true;
        }
        else if (dy <= -0.5)
        {
            currPos[0]--;
            dy += 1;
            changed = true;
        }
    }
    else if (dx * vel[0] >= 0 && dy * vel[1] >= 0)
    {
        dx = dy = vel[0] = vel[1] = 0;
        changed = false;
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

    if (dir >= 0 && dir < 4)
    {
        ready = false;
    }
}

void Character::turn(int dir)
{
    if (!active)
    {
        return;
    }

    changed = false;
    ready = true;

    dx = dy = 0;
    vel[0] = vel[1] = 0;

    if (dir > 3 || dir < 0)
    {
        return;
    }

    currDir = dir;

    switch (dir)
    {
        case 0: spriteRect = spriteDim[3][1]; break;
        case 1: spriteRect = spriteDim[2][1]; break;
        case 2: spriteRect = spriteDim[0][1]; break;
        case 3: spriteRect = spriteDim[1][1]; break;
        default: break;
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

        int points = (int) std::round((prop.power / 10.0 + prop.health / 3000.0 + prop.speed / 10.0) / 3.0 * 10);

        if (isMyTeam)
        {
            opponentTeamScore += points;
        }
        else
        {
            myTeamScore += points;
        }
    }
}
