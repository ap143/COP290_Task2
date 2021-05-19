#include "maze.hpp"

Maze::Maze(int size, int w, int h)
{
    n = size;

    // Initializing maze
    maze = new bool **[n];
    for (int i = 0; i < n; i++)
    {
        maze[i] = new bool *[n];
        for (int j = 0; j < n; j++)
        {
            maze[i][j] = new bool[4];
            for (int k = 0; k < 4; k++)
            {
                maze_health.push_back(wall_health * 300);
            }
        }
    }

    // Setting max health
    max_health = wall_health * 300;

    // computing maze length and cell length according to window width and height
    h = std::min(w, h);

    cell_size = ((n - 1) * ((float)h) / n) / n;

    grid_length = (n - 1) * ((float)h) / n;

    ox = oy = (float)(h) / 2 - (float)grid_length / 2;

    // Memeset
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                maze[i][j][k] = 0;
            }
        }
    }
}

Maze::~Maze()
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            delete maze[i][j];
        }
        delete maze[i];
    }

    if (grass != NULL)
    {
        SDL_DestroyTexture(grass);
    }
    if (wall != NULL)
    {
        SDL_DestroyTexture(wall);
    }
}

void Maze::generate()
{
    // For storing which cells are visited
    std::vector<std::vector<bool>> cells(n, std::vector<bool>(n, false));

    // Randomising generated maze
    srand(time(0));

    // Genrating maze
    dfs(cells, n / 2, n / 2);
}

//Healper function to return random direction to move
int myrandom(int i) { return std::rand() % i; }

void Maze::dfs(std::vector<std::vector<bool>> &cells, int i, int j)
{
    // Current cell visited
    cells[i][j] = true;

    // Set of directions to move from current cell
    std::vector<int> s;

    for (int k = 0; k < 4; k++)
        s.push_back(k);

    // Random Shuffling of set
    random_shuffle(s.begin(), s.end(), myrandom);

    // Moving into next cell
    for (unsigned int k = 0; k < s.size(); k++)
    {
        int id = s[k];

        switch (id)
        {
        case 0:
            if (i - 1 >= 0 && !cells[i - 1][j])
            {
                maze[i - 1][j][2] = true;
                maze[i][j][0] = true;
                dfs(cells, i - 1, j);
                break;
            }
        case 1:
            if (j + 1 < n && !cells[i][j + 1])
            {
                maze[i][j + 1][3] = true;
                maze[i][j][1] = true;
                dfs(cells, i, j + 1);
                break;
            }
        case 2:
            if (i + 1 < n && !cells[i + 1][j])
            {
                maze[i + 1][j][0] = true;
                maze[i][j][2] = true;
                dfs(cells, i + 1, j);
                break;
            }
        case 3:
            if (j - 1 >= 0 && !cells[i][j - 1])
            {
                maze[i][j - 1][1] = true;
                maze[i][j][3] = true;
                dfs(cells, i, j - 1);
                break;
            }
        }
    }
}

void Maze::show(SDL_Renderer *renderer, SDL_Window *window)
{
    // Background image for current state of game
    backgroundImage(renderer, grass);

    // Setting disply size of wall texture
    float ww = cell_size;
    float wh = 2 * cell_size / 5;

    // Wall health bar size
    float bar_length = cell_size * 2 / 3;

    // Index of current wall in Maz_health vector
    int index = 0;

    // Drawing maze
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < 2; k++)
            {

                if (maze[i][j][k])
                {
                    continue;
                }
                if (k == 0)
                {
                    imageCenter(renderer, wall, NULL, ox + cell_size * j + cell_size / 2, oy + cell_size * i,
                                ww, wh, 0, SDL_FLIP_NONE);
                }
                else if (k == 1)
                {
                    imageCenter(renderer, wall, NULL, ox + cell_size * (j + 1), oy + cell_size * i + cell_size / 2,
                                ww, wh, 270, SDL_FLIP_NONE);
                }
            }
            if (j == 0)
            {
                if (!maze[i][j][3])
                {
                    imageCenter(renderer, wall, NULL, ox + cell_size * j, oy + cell_size * i + cell_size / 2,
                                ww, wh, 270, SDL_FLIP_NONE);
                }
            }

            if (i == n - 1)
            {
                if (!maze[i][j][2])
                {
                    imageCenter(renderer, wall, NULL, ox + cell_size * j + cell_size / 2, oy + cell_size * (i + 1),
                                ww, wh, 0, SDL_FLIP_NONE);
                }
            }
        }
    }

    // Drawing wall health if less than max_health
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < 2; k++)
            {
                index = i * n * 4 + j * 4 + k;

                if (maze[i][j][k])
                {
                    continue;
                }

                int red, green;

                if ((float)maze_health[index] / max_health >= 0.5)
                {
                    green = 255;
                    red = std::min(255, (int)(255 * (1 - (float)maze_health[index] / max_health) * 2));
                }
                else
                {
                    red = 255;
                    green = std::min(255, (int)(255 * ((float)maze_health[index] / max_health) * 2));
                }

                color(renderer, red, green, 0);

                if (k == 0)
                {
                    if (maze_health[index] < max_health)
                    {
                        rect(renderer, ox + cell_size * j + cell_size * 3 / 4, oy + cell_size * i, bar_length * maze_health[index] / max_health, wh / 8, true);
                    }
                }
                else if (k == 1)
                {
                    if (maze_health[index] < max_health)
                    {
                        rect(renderer, ox + cell_size * (j + 1), oy + cell_size * i, bar_length * maze_health[index] / max_health, wh / 8, true);
                    }
                }
            }
        }
    }
}
