#include "teamView.hpp"

Teamview::Teamview(SDL_Renderer *rend, Maze *maze, int teamN, bool self)
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

    block_ox = game_maze->ox + game_maze->grid_length * 1.05;
    block_oy = 5 * gui_height / 8;
    block_width = gui_width - block_ox;
    block_height = gui_height - block_oy;

    src = {.x = 48, .y = 0, .w = 48, .h = 49};

    tile_height = block_width / 2 - block_width / 8;
    bar_height = block_height/ 4;
    bar_width = (block_width - tile_height) * 0.9;
    bar_fill = 0;

    for (int i = 0; i < 4; i++)
    {
        tiles[i] = {.x = block_ox, .y = block_oy + (i / 2) * (5 * tile_height / 4), .w = tile_height, .h = tile_height};
        tiles[i+1] = {.x = block_ox + tile_height * 5 / 4, .y = block_oy + (i / 2) * (5 * tile_height / 4), .w = tile_height, .h = tile_height};
        count_text[i] = text(renderer, "x" + std::to_string(count[i]), count_text_size);
        count_text[i+1] = text(renderer, "x" + std::to_string(count[i+1]), count_text_size);
        i++;
    }
}

Teamview::~Teamview()
{
    for (std::vector<Character*> v : characters)
    {
        for (Character* c : v)
        {
            delete c;
        }
    }

    for (SDL_Texture* s : count_text)
    {
        if (s != NULL)
        {
            SDL_DestroyTexture(s);
        }
    }

    if (tile_tex != NULL)
    {
        SDL_DestroyTexture(tile_tex);
    }
}

void Teamview::show()
{
    if (isMyTeam)
    {
        if (activeLevel == 0 && !kingDeployed)
        {
            color(renderer, 0, 255, 0, 100);
            rect(renderer, game_maze->ox + (game_maze->n - deployRange) * game_maze->cell_size, game_maze->oy + (game_maze->n - deployRange) * game_maze->cell_size, deployRange * game_maze->cell_size, deployRange * game_maze->cell_size, true);
        }

        if (deployingNow)
        {
            Character *king = characters[0][0];
            float x = (king->currPos[1] + 0.5) * game_maze->cell_size + game_maze->ox;
            float y = (king->currPos[0] + 0.5) * game_maze->cell_size + game_maze->oy;

            float length = (2 * (deployRange / 2) + 1) * game_maze->cell_size;

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

        for (int i = 0; i < 4; i++)
        {
            if (activeLevel == i && count[i] > 0)
            {
                color(renderer, 217, 59, 11, 160);
                rectCenter(renderer, tiles[i].x + tile_height / 2, tiles[i].y + tile_height / 2, tile_height, tile_height, 1.05, true);
            }
            else
            {
                color(renderer, 217, 179, 11, 255);
                rectCenter(renderer, tiles[i].x + tile_height / 2, tiles[i].y + tile_height / 2, tile_height, tile_height, 1.05, true);
            }

            // Tile on which sprite placed
            color(renderer, 0, 255);
            rectCenter(renderer, tiles[i].x + tile_height / 2, tiles[i].y + tile_height / 2, tile_height, tile_height, 0.95, true);


            // Sprite
            imageCenter(renderer, characters[i][0]->spriteSheet, &src, tiles[i].x + tile_height / 2, tiles[i].y + tile_height / 2, tile_height, tile_height, 0.9);
            if (!kingDeployed && !opponentKingDeployed && !game_over && i >= 1)
            {
                color(renderer, 0, 0, 0, 160);
                rectCenter(renderer, tiles[i].x + tile_height / 2, tiles[i].y + tile_height / 2, tile_height, tile_height, 1.05, true);
            }
            // Text
            color(renderer, 255, 255, 255, 255);
            rectCenter(renderer, tiles[i].x, tiles[i].y, 20, 20, 1, true);
            imageCenter(renderer, count_text[i], NULL, tiles[i].x, tiles[i].y, (float)20, (float)20);


            if (count[i] == 0)
            {
                // Shade when count of a level = 0
                color(renderer, 0, 160);
                rectCenter(renderer, tiles[i].x + tile_height / 2, tiles[i].y + tile_height / 2, tile_height, tile_height, 0.95, true);
            }
        }
    }

    for (std::vector<Character *> v : characters)
    {
        for (Character *c : v)
        {
            if (c->active && !c->dead)
            {
                c->show();
            }
        }
    }
}

void Teamview::handleEvent(SDL_Event event)
{

    if (game_over)
    {
        return;
    }

    if (event.type == SDL_KEYDOWN && opponentKingDeployed && kingDeployed)
    {
        // Move king
        Character *king = characters[0][0];
        
        king->attackDir = -1;

        if (!king->ready)
        {
            return;
        }

        int dir = -1;
        bool move = true;

        if (event.key.keysym.sym == SDLK_a)
        {
            kingAttack(king);
        }

        switch (event.key.keysym.sym)
        {
        case SDLK_UP:
            dir = 0;
            break;
        case SDLK_DOWN:
            dir = 2;
            break;
        case SDLK_RIGHT:
            dir = 1;
            break;
        case SDLK_LEFT:
            dir = 3;
            break;
        default:
            break;
        }

        if (dir == -1)
        {
            return;
        }

        if (SDL_GetModState() & (SDLK_RSHIFT | SDLK_LSHIFT))
        {
            king->turn(dir);
        }
        else
        {
            king->setVel(dir);
        }

        if (std::abs(king->vel[0]) + std::abs(king->vel[1]) == 0)
        {
            move = false;
        }

        if (move)
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
            if ((!opponentKingDeployed || !kingDeployed) && i > 0)
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
        }
        else if (std::abs(characters[0][0]->currPos[0] - i) > deployRange / 2 || std::abs(characters[0][0]->currPos[1] - j) > deployRange / 2)
        {
            deployingNow = false;
            activeLevel = -1;
            return;
        }

        characters[activeLevel][count[activeLevel] - 1]->deploy(i, j);
        count[activeLevel]--;
        
        if(count_text[activeLevel] != NULL)
        {
            SDL_DestroyTexture(count_text[activeLevel]);
        }

        count_text[activeLevel] = text(renderer, "x" + std::to_string(count[activeLevel]), count_text_size);

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
    if (game_over)
    {
        return;
    }

    for (std::vector<Character *> v : characters)
    {
        int j = -1;
        for (Character *c : v)
        {
            j++;
            if (c->ready && isMyTeam && c->level > 0 && c->active)
            {
                setNextDest(c, c->level, j);
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

void Teamview::setNextDest(Character *c, int level, int cnt)
{
    int ci = c->currPos[0];
    int cj = c->currPos[1];
    int n = game_maze->n;
    Point *graph[n][n];

    std::vector<Point *> qu;

    graph[ci][cj] = new Point(ci, cj, 0, false, false);
    qu.push_back(graph[ci][cj]);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i != ci || j != cj)
            {
                graph[i][j] = new Point(i, j, 2147483647, false, false);
                qu.push_back(graph[i][j]);
            }
        }
    }

    for (unsigned int i = 0; i < enemyTeam->characters.size(); i++)
    {
        for (unsigned int j = 0; j < enemyTeam->characters[i].size(); j++)
        {
            Character *e = enemyTeam->characters[i][j];
            if (!e->active)
            {
                continue;
            }
            Point *p = graph[e->currPos[0]][e->currPos[1]];
            p->enemy = true;
            if (p->e == nullptr || rand() % 2 == 0)
            {
                p->e = e;
                p->lvl = i;
                p->cnt = j;
            }
        }
    }

    Point *target = nullptr;

    while (!qu.empty())
    {
        Point *p = qu[0];
        int ind = 0;

        for (unsigned int i = 0; i < qu.size(); i++)
        {
            if (qu[i]->dist < p->dist)
            {
                p = qu[i];
                ind = i;
            }
        }

        qu.erase(qu.begin() + ind);

        p->visited = true;

        if (p->enemy)
        {
            target = p;
            break;
        }

        int pi = p->i;
        int pj = p->j;

        int pos[4][2] = {{pi - 1, pj}, {pi, pj + 1}, {pi + 1, pj}, {pi, pj - 1}};

        for (int k = 0; k < 4; k++)
        {
            int i = pos[k][0], j = pos[k][1];
            if (i < 0 || i >= n || j < 0 || j >= n)
            {
                continue;
            }
            if (graph[i][j]->visited)
            {
                continue;
            }

            int newDist = -1;
            if (game_maze->maze[pi][pj][k])
            {
                newDist = 1;
            }
            else
            {
                newDist = game_maze->wall_health;
            }

            if (graph[i][j]->dist > newDist + p->dist)
            {
                graph[i][j]->dist = newDist + p->dist;
                graph[i][j]->pre = p;
            }
        }
    }

    if (target == nullptr)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                delete graph[i][j];
            }
        }
        return;
    }

    while (true)
    {
        if (target->pre == nullptr)
        {
            break;
        }
        else
        {
            if (target->pre->pre == nullptr)
            {
                break;
            }
            target = target->pre;
        }
    }

    int ti = target->i;
    int tj = target->j;

    int dir = -1;

    if (ci == ti && cj == tj)
    {
        sendMessage(ATTACK + std::to_string(target->lvl) + std::to_string(target->cnt) +
                    std::string((c->prop.power < 10) ? "0" : "") + std::to_string(c->prop.power) +
                    std::to_string(level) + std::to_string(cnt) + std::to_string(4));
        c->attackDir = 4;
        target->e->attack(c->prop.power);
    }
    else if (ci - 1 == ti)
    {
        dir = 0;
    }
    else if (cj + 1 == tj)
    {
        dir = 1;
    }
    else if (ci + 1 == ti)
    {
        dir = 2;
    }
    else if (cj - 1 == tj)
    {
        dir = 3;
    }
    else
    {

    }

    if (dir != -1)
    {
        if (!game_maze->maze[ci][cj][dir])
        {
            sendMessage(BREAK_WALL + std::string((ci < 10) ? "0" : "") + std::to_string(ci) +
                        std::string((cj < 10) ? "0" : "") + std::to_string(cj) + std::to_string(dir) +
                        std::string((c->prop.power < 10) ? "0" : "") + std::to_string(c->prop.power) +
                        std::to_string(level) + std::to_string(cnt) + std::to_string(dir));
            attackWall(ci, cj, dir, c->prop.power);
            c->attackDir = dir;
            c->turn(dir);
        }
        else if (target->enemy)
        {
            sendMessage(ATTACK + std::to_string(target->lvl) + std::to_string(target->cnt) +
                        std::string((c->prop.power < 10) ? "0" : "") + std::to_string(c->prop.power) +
                        std::to_string(level) + std::to_string(cnt) + std::to_string(dir));
            target->e->attack(c->prop.power);
            c->turn(dir);
            c->attackDir = dir;
        }
        else
        {
            sendMessage(MOVEMENT + std::to_string(level) + std::to_string(cnt) + std::to_string(dir));
            c->setVel(dir);
            c->attackDir = -1;
        }
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            delete graph[i][j];
        }
    }
}

void Teamview::attackWall(int i, int j, int dir, int power)
{
    if (i == 0 && dir == 0)
    {
        return;
    }
    else if (i == game_maze->n - 1 && dir == 2)
    {
        return;
    }
    else if (j == 0 && dir == 3)
    {
        return;
    }
    else if (j == game_maze->n - 1 && dir == 1)
    {
        return;
    }

    int index = i * game_maze->n * 4 + j * 4 + dir;
    game_maze->maze_health[index] -= power;

    if (dir == 0 && i > 0)
    {
        game_maze->maze_health[index - game_maze->n * 4 + 2] -= power;
    }
    else if (dir == 1 && j < game_maze->n - 1)
    {
        game_maze->maze_health[index + 4 + 2] -= power;
    }
    else if (dir == 2 && i < game_maze->n - 1)
    {
        game_maze->maze_health[index + game_maze->n * 4 - 2] -= power;
    }
    else if (dir == 3 && i > 0)
    {
        game_maze->maze_health[index - 4 - 2] -= power;
    }

    if (game_maze->maze_health[index] <= 0)
    {
        game_maze->maze[i][j][dir] = true;
        if (dir == 0 && i > 0)
        {
            game_maze->maze[i - 1][j][2] = true;
        }
        else if (dir == 1 && j < game_maze->n - 1)
        {
            game_maze->maze[i][j + 1][3] = true;
        }
        else if (dir == 2 && i < game_maze->n - 1)
        {
            game_maze->maze[i + 1][j][0] = true;
        }
        else if (dir == 3 && i > 0)
        {
            game_maze->maze[i][j - 1][1] = true;
        }
    }
}

void Teamview::kingAttack(Character *c)
{
    int ci = c->currPos[0];
    int cj = c->currPos[1];

    int pos[2] = {ci, cj};

    switch (c->currDir)
    {
    case 0:
        pos[0]--;
        break;
    case 1:
        pos[1]++;
        break;
    case 2:
        pos[0]++;
        break;
    case 3:
        pos[1]--;
        break;
    default:
        break;
    }

    Character *target = nullptr;
    int cnt = -1;
    bool onPlace = false;

    for (std::vector<Character *> v : enemyTeam->characters)
    {
        int j = -1;
        for (Character *e : v)
        {
            j++;
            if (!e->active || e->dead)
            {
                continue;
            }
            if (e->currPos[0] == ci && e->currPos[1] == cj)
            {
                target = e;
                cnt = j;
                onPlace = true;
                break;
            }
            if (e->currPos[0] == pos[0] && e->currPos[1] == pos[1])
            {
                if (target == nullptr || rand() % 2 == 0)
                {
                    target = e;
                    cnt = j;
                }
            }
        }
    }

    if (target == nullptr)
    {
        return;
    }

    if (onPlace)
    {
        target->attack(c->prop.power);
        sendMessage(ATTACK + std::to_string(target->level) + std::to_string(cnt) +
                    std::string((c->prop.power < 10) ? "0" : "") + std::to_string(c->prop.power) +
                    std::to_string(0) + std::to_string(0) + std::to_string(c->currDir));
        c->attackDir = 4;
    }
    else if (!game_maze->maze[ci][cj][c->currDir])
    {
        attackWall(ci, cj, c->currDir, c->prop.power);
        sendMessage(BREAK_WALL + std::string((ci < 10) ? "0" : "") + std::to_string(ci) +
                    std::string((cj < 10) ? "0" : "") + std::to_string(cj) + std::to_string(c->currDir) +
                    std::string((c->prop.power < 10) ? "0" : "") + std::to_string(c->prop.power) +
                    "00" + std::to_string(c->currDir));
        c->attackDir = c->currDir;
    }
    else if (target != nullptr)
    {
        target->attack(c->prop.power);
        sendMessage(ATTACK + std::to_string(target->level) + std::to_string(cnt) +
                    std::string((c->prop.power < 10) ? "0" : "") + std::to_string(c->prop.power) +
                    std::to_string(0) + std::to_string(0) + std::to_string(c->currDir));
        c->attackDir = c->currDir;
    }
}
