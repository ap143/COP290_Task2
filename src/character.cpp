#include "character.hpp"

Character::Character()
{

}

Character::Character(int teamN, int lev, bool t)
{
    team = teamN;
    level = lev;
    isMyTeam = t;
    currPos[0] = 0;
    currPos[1] = 0;
    prop.speed = 4;
    vel[0] = 0;
    vel[1] = prop.speed;

    spriteSheet = loadTexture(("./assets/images/characters/t" + std::to_string(team+1) + "l" + std::to_string(lev) + ".png").c_str(), renderer);
}

void Character::show()
{
    SDL_RenderCopyF(renderer, spriteSheet, &spriteRect, &posRect);
}
