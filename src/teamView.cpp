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

        if (deployingNow)
        {
            Character *king = characters[0][0];
            float x = (king->currPos[1] + 0.5) * game_maze->cell_size + game_maze->ox;
            float y = (king->currPos[0] + 0.5) * game_maze->cell_size + game_maze->oy;
            
            float length = ( 2 * (deployRange / 2) + 1 ) * game_maze->cell_size;

            float width = length;
            float height = length;

            x -= (deployRange / 2 + 0.5) * game_maze->cell_size;
            y -= (deployRange / 2 + 0.5) * game_maze->cell_size;

            if (x < game_maze->ox)
            {
                width -= (game_maze->ox - x);
                x = game_maze->ox;
            }
            else if (x + width >= game_maze->ox + game_maze->grid_length)
            {
                width -= (x + width - (game_maze->ox + game_maze->grid_length));
            }

            if (y < game_maze->oy)
            {
                height -= (game_maze->oy - y);
                y = game_maze->oy;
            }
            else if (y + height >= game_maze->oy + game_maze->grid_length)
            {
                height -= (y + height - (game_maze->oy + game_maze->grid_length));
            }
            
            color(renderer, 0, 255, 0, 100);
            rect(renderer, x, y, width, height, true);
        }
        
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
            if (i > 0)
            {
                deployingNow = true;
            }
            return;
        }
    }

    if (activeLevel == -1)
    {
        deployingNow = false;
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
        }else if (std::abs(characters[0][0]->currPos[0] - i) > deployRange / 2 || std::abs(characters[0][0]->currPos[1] - j) > deployRange / 2)
        {
            deployingNow = false;
            activeLevel = -1;
            return;
        }

        characters[activeLevel][count[activeLevel]-1]->deploy(i, j);
        count[activeLevel]--;

        sendMessage(DEPLOY + std::to_string(activeLevel) + std::to_string(count[activeLevel]) + ((i < 10) ? "0" : "") + std::to_string(i) + ((j < 10) ? "0" : "") + std::to_string(j));

        if (count[activeLevel] == 0)
        {
            activeLevel = -1;
            deployingNow = false;
        }

        kingDeployed = true;
    }
    else
    {
        activeLevel = -1;
        deployingNow = false;
    }

}

void Teamview::update()
{
    for (std::vector<Character *> v: characters)
    {
        for (Character *c: v)
        {
            if (c->ready && isMyTeam && c->level > 0)
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

struct Point
{
    int i, j;
    int dist;
    bool enemy, visited;

    Point(int i, int j, int dist, bool enemy, bool visited)
    {
        this->i = i;
        this->j = j;
        this->dist = dist;
        this->enemy = enemy;
        this->visited = visited;
    }
};

struct Compare
{
    bool operator()(Point* const& p1, Point* const& p2)
    {
        return p1->dist < p2->dist;
    }
};

int exactToRelative(int pos, bool column, Character *c)
{
    return pos - c->currPos[column] + c->prop.range;
}

int relativeToExact(int pos, bool column, Character *c)
{
    return pos + c->currPos[column] - c->prop.range;
}

void Teamview::setNextDest(Character *c)
{
    int range = 2 * c->prop.range + 1;
    Point* dist[range][range];

    for (int i = 0; i < range; i++)
    {
        for (int j = 0; j < range; j++)
        {
            dist[i][j] = new Point(relativeToExact(i, false, c), relativeToExact(j, true, c), -1, false, false);
        }
    }

    int lvl = -1;
    int cnt = -1;
    for (std::vector<Character *> v: enemyTeam->characters)
    {
        lvl++;
        for (Character *e: v)
        {
            cnt++;
            int i = e->currPos[0]-c->currPos[0];
            int j = e->currPos[1]-c->currPos[1];

            if ((i == j && i == 0) || (std::abs(i) + std::abs(j) == 1))
            {
                e->attack(c->prop.power);
                sendMessage(ATTACK + std::to_string(lvl) + std::to_string(cnt) + std::to_string(c->prop.power));
                for (int ii = 0; ii < range; ii++)
                {
                    for (int jj = 0; jj < range; jj++)
                    {
                        delete dist[ii][jj];
                    }
                }
                return;
            }

            i += range / 2;
            j += range / 2;

            if (i >= 0 && i < range && j >= 0 && j < range)
            {
                dist[i][j]->enemy = true;
            }
        }
    }

    std::priority_queue<Point*, std::vector<Point*>, Compare> qu;

    qu.push(dist[range / 2][range / 2]);

    while (!qu.empty())
    {
        Point* p = qu.top();
        qu.pop();
        p->visited = true;

        if (game_maze->maze[p->i][p->j][0])
        {
            int tj = exactToRelative(p->j, true, c);
            int ti = exactToRelative(p->i - 1, false, c);
            if (tj >= 0)
            {
                if (distp[][tj])
            }
        }
    }

    for (int ii = 0; ii < range; ii++)
    {
        for (int jj = 0; jj < range; jj++)
        {
            delete dist[ii][jj];
        }
    }
}
