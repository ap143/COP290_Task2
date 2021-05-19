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

Character::Character(SDL_Renderer *renderer, Maze *maze, int teamN, int lev, bool t)
{
    this->renderer = renderer;

    game_maze = maze;

    team = teamN;
    level = lev;
    isMyTeam = t;

    // Game Play Properties of character
    prop = Character::character_data[level * Character::total_teams + team];
    health = prop.health;
    if (level == 0)
    {
        prop.power *= 5;
    }

    spriteSheet = loadTexture(("./assets/images/characters/t" + std::to_string(team + 1) + "l" + std::to_string(lev) + ".png").c_str(), renderer);
    attackSheet = loadTexture(("./assets/images/characters/t" + std::to_string(team + 1) + "l" + std::to_string(lev) + "_blast.png").c_str(), renderer);

    // Attack sheet src rectangle
    SDL_QueryTexture(attackSheet, NULL, NULL, &w, &h);
    blastRect = {.x = 0, .y = 0, w / 6, h};
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
    //  If player is not active or dead
    if (!active)
    {
        return;
    }

    // If attacking then according to attack dir drawing blast texture
    if (attackDir == 0)
    {
        imageCenter(renderer, attackSheet, &blastRect, posRect.x, posRect.y - game_maze->cell_size / 3, attackW, attackH);
    }
    else if (attackDir == 1)
    {
        imageCenter(renderer, attackSheet, &blastRect, posRect.x + game_maze->cell_size / 3, posRect.y, attackW, attackH);
    }
    else if (attackDir == 2)
    {
        imageCenter(renderer, attackSheet, &blastRect, posRect.x, posRect.y + game_maze->cell_size / 3, attackW, attackH);
    }
    else if (attackDir == 3)
    {
        imageCenter(renderer, attackSheet, &blastRect, posRect.x - game_maze->cell_size / 3, posRect.y, attackW, attackH);
    }
    else if (attackDir == 4)
    {
        imageCenter(renderer, attackSheet, &blastRect, posRect.x, posRect.y, attackW, attackH);
    }

    attackDir = -1;

    // Current sprite of character
    imageCenter(renderer, spriteSheet, &spriteRect, posRect.x, posRect.y, posRect.w, posRect.h);

    // Health bar of charater

    // Bar color
    int red, green;
    if (health / prop.health >= 0.5)
    {
        green = 255;
        red = std::min(255, (int)(255 * (1 - health / prop.health) * 2));
    }
    else
    {
        red = 255;
        green = std::min(255, (int)(255 * (health / prop.health) * 2));
    }
    color(renderer, red, green, 0);

    // Bar show
    rect(renderer, posRect.x + posRect.w / 2, posRect.y - posRect.h / 2, posRect.w * health / prop.health, posRect.h / 16, true);
}

void Character::deploy(int i, int j)
{
    // Setting current position in maze
    currPos[0] = i;
    currPos[1] = j;

    // Initializing velocity
    vel[0] = vel[1] = 0;
    currDir = isMyTeam ? 3 : 1;

    // Setting up boolean variables
    active = true;
    ready = true;

    // Selecting sprite src Rect according to current facing direction
    spriteRect = currDir == 3 ? spriteDim[1][1] : spriteDim[2][1];

    // Character loaction on screen
    posRect = {.x = game_maze->ox + currPos[1] * game_maze->cell_size + game_maze->cell_size / 2,
               .y = game_maze->oy + currPos[0] * game_maze->cell_size + game_maze->cell_size / 2,
               .w = game_maze->cell_size * (float)0.8,
               .h = game_maze->cell_size * (float)0.8};

    // Blast Texture height and width
    attackW = posRect.w / 1.2;
    attackH = posRect.h / 1.2;

    if (level == 0)
    {
        Mix_PlayChannel(-1, king_deploy, 0);
    }
    else if (isMyTeam)
    {
        Mix_PlayChannel(-1, friend_deploy, 0);
    }
    else
    {
        Mix_PlayChannel(-1, enemy_deploy, 0);
    }
}

void Character::update()
{
    // If not active or dead then return
    if (!active)
    {
        return;
    }

    // Change of sprite according to frame and movement
    if (sprite_change == max_speed + 1 - prop.speed)
    {
        sprite_change = 0;
        // Sprite update
        if (vel[0] > 0)
        {
            spriteRect = spriteDim[2][currSprite];
            currSprite = (currSprite + 1) % 3;
        }
        else if (vel[0] < 0)
        {
            spriteRect = spriteDim[1][currSprite];
            currSprite = (currSprite + 1) % 3;
        }
        else if (vel[1] > 0)
        {
            spriteRect = spriteDim[0][currSprite];
            currSprite = (currSprite + 1) % 3;
        }
        else if (vel[1] < 0)
        {
            spriteRect = spriteDim[3][currSprite];
            currSprite = (currSprite + 1) % 3;
        }
    }
    else
    {
        sprite_change++;
    }

    // Blast src rect in attack sheet according to frame change
    if (attackDir != -1)
    {
        blastRect.x = currBlast * blastRect.w;
        currBlast = (currBlast + 1) % 6;
    }

    // Health update due to heal
    health = std::min(prop.health, health + prop.heal);

    //Pos Update
    dx += vel[0] * velConst * prop.speed;
    dy += vel[1] * velConst * prop.speed;

    posRect.x = game_maze->ox + currPos[1] * game_maze->cell_size +
                game_maze->cell_size / 2 + dx * game_maze->cell_size;
    posRect.y = game_maze->oy + currPos[0] * game_maze->cell_size +
                game_maze->cell_size / 2 + dy * game_maze->cell_size;

    // Curr Cell change accoridng relative displacement from centre of cell 
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
    // If not active or dead then return
    if (!active)
    {
        return;
    }

    // To change direction  
    turn(dir);

    // If wall in front of current facing direction then return 
    if (!game_maze->maze[currPos[0]][currPos[1]][dir])
    {
        return;
    }

    // Setting vel according to new current direction
    switch (dir)
    {
    case 0:
        vel[1] = -1;
        break;
    case 1:
        vel[0] = 1;
        break;
    case 2:
        vel[1] = 1;
        break;
    case 3:
        vel[0] = -1;
        break;
    default:
        break;
    }

    if (dir >= 0 && dir < 4)
    {
        ready = false;
    }
}

void Character::turn(int dir)
{
    // If not active or dead then return
    if (!active)
    {
        return;
    }

    // Currently at centre of cell
    changed = false;
    // Ready to move in new direction
    ready = true;

    // Relative distance = 0 from centre of cell as currently at centre
    dx = dy = 0;

    // new velocity to be set
    vel[0] = vel[1] = 0;


    if (dir > 3 || dir < 0)
    {
        return;
    }

    // Changing curre Direction to new Direction
    currDir = dir;

    // Setting src sprite rectangle according to new direction
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
    // Updating health
    health -= power;

    // Checking if character is dead
    if (health <= 0)
    {
        active = false;
        dead = true;

        // If it is king then game is over
        if (level == 0)
        {
            Mix_PlayChannel(-1, king_die, 0);
            game_over = true;
        }

        // Updating score of Teams
        int points = (int)std::round((prop.power / 10.0 + prop.health / 3000.0 + prop.speed / 10.0 + prop.heal / 3.0) / 4.0 * 10);

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
