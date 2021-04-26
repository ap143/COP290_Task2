#include "character.hpp"

Character::Character()
{

}

Character::Character(int lev, bool t)
{
    level = lev;
    team = t;
    currPos[0] = 0;
    currPos[1] = 0;
    prop.speed = 4;
    vel[0] = 0;
    vel[1] = prop.speed;
}

void Character::show()
{
    SDL_RenderCopyF(renderer, spriteSheet, &spriteRect, &posRect);
}

void Character::updatePos()
{
    std::cout <<dx<<" "<<dy<<" "<<gameMaze->cell_size<<std::endl;
    if (dx >= 1){
        currPos[0] += 1;
        dx = 0;
    }else if (dy >= 1) {
        currPos[1] += 1;
        dy = 0;
    }else if (dx <= -1){
        currPos[0] -= 1;
        dx = 0;
    }else if (dy <= -1){
        currPos[1] -= 1;
        dy = 0;
    }else{
        //std::cout<<"returned"<<std::endl;
        return;
    }

    currPos[0] = (gameMaze->n + currPos[0])%(gameMaze->n);
    currPos[1] = (gameMaze->n + currPos[1])%(gameMaze->n);

    std::cout<< currPos[0] <<" "<<currPos[1]<<std::endl;

    if (gameMaze->maze[currPos[0]][currPos[1]][0]){
        vel[0] = 0;
        vel[1] = -prop.speed; 
    }else if (gameMaze->maze[currPos[0]][currPos[1]][1]){
        vel[0] = prop.speed;
        vel[1] = 0;
    } else if (gameMaze->maze[currPos[0]][currPos[1]][2]){
        vel[0] = 0;
        vel[1] = prop.speed;
    } else if(gameMaze->maze[currPos[0]][currPos[1]][3]){
        vel[0] = -prop.speed;
        vel[1] = 0;
    }
}

void Character::move(SDL_Renderer* ren)
{
    dx += (velConst * (float)vel[0]);
    dy += (velConst * (float)vel[1]);

    //std::cout<<dx <<" "<<dy<<std::endl;

    posRect.x += (velConst * (float)vel[0])*(gameMaze->cell_size);
    posRect.y += (velConst * (float)vel[1])*(gameMaze->cell_size);

    int dir;
    if (vel[0] > 0) dir = 1;
    else if (vel[0] < 0) dir = 3;
    else if (vel[1] < 0) dir = 0;
    else if (vel[1] > 0) dir = 2;
    else dir = -1;

    drawCurrSprite(dir);
}

void Character::drawCurrSprite(int dir)
{
    spriteRect.x = spriteRect.w * currSprite;

    currSprite = (currSprite + 1)%3;

    if (dir == 0) spriteRect.y = spriteRect.h * 3;
    else if (dir == 1) spriteRect.y = spriteRect.h * 2;
    else if (dir == 2) spriteRect.y = spriteRect.h * 0;
    else if (dir == 3) spriteRect.y = spriteRect.h * 1;
    else ;

    imageCenter(renderer, spriteSheet, &spriteRect, posRect.x, posRect.y, posRect.w, posRect.h);
}

void Character::setupSprite(SDL_Texture* tex, SDL_Renderer* ren)
{
    spriteSheet = tex;
    renderer = ren;
    
    spriteRect.x = 0;
    spriteRect.y = 0;
    
    SDL_QueryTexture(spriteSheet, NULL, NULL, &(spriteRect.w), &(spriteRect.h));

    spriteRect.w /= 3;
    spriteRect.h /= 4;

    posRect.x = gameMaze->ox + currPos[1]*(gameMaze->cell_size) + (gameMaze->cell_size)/2;
    posRect.y = gameMaze->oy + currPos[0]*(gameMaze->cell_size) + (gameMaze->cell_size)/2;
    posRect.w = gameMaze->cell_size;
    posRect.h = gameMaze->cell_size;
}