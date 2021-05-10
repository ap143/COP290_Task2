#include "maze.hpp"

Maze::Maze(int size, int w, int h)
{
    n = size;
    maze = new bool **[n];
    for (int i = 0; i < n; i++)
    {
        maze[i] = new bool *[n];
        for (int j = 0; j < n; j++)
        {
            maze[i][j] = new bool[4];
            for (int k = 0; k < 4; k++)
            {
                maze_health.push_back(wall_health);
            }
        }
    }

    h = std::min(w, h);

    cell_size = ((n-1) * ((float)h) / n) / n;

    grid_length = (n-1) * ((float)h) / n;
    //rect(renderer,(w)/2- grid_length/2, (h)/2- grid_length/2, grid_length, grid_length, 0);

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

void Maze::generate()
{
    std::vector<std::vector<bool>> cells(n, std::vector<bool>(n, false));
    srand(time(0));
    dfs(cells, n / 2, n / 2);
}

int myrandom(int i) { return std::rand() % i; }

void Maze::dfs(std::vector<std::vector<bool>> &cells, int i, int j)
{
    cells[i][j] = true;
    std::vector<int> s;

    // if (i-1>=0 && !cells[i-1][j]) s.insert(0);
    // if (j+1<n && !cells[i][j+1]) s.insert(1);
    // if (i+1<n && !cells[i+1][j]) s.insert(2);
    // if (j-1>=0 && !cells[i][j-1]) s.insert(3);

    for (int k = 0; k < 4; k++)
        s.push_back(k);

    random_shuffle(s.begin(), s.end(), myrandom);

    // for (int k = 0; k<4;k++) std::cout<< s[k] << " ";
    // std::cout <<std::endl;

    for (unsigned int k = 0; k < s.size(); k++)
    {

        // srand(time(NULL));

        int id = s[k];
        // while(s.find(id)== s.end()) {
        //     srand(time(0));
        //     id  = rand()%4;
        //     std::cout<<id<<std::endl;
        // }

        switch (id)
        {
        case 0:
            if (i - 1 >= 0 && !cells[i - 1][j])
            {
                maze[i - 1][j][2] = true;
                maze[i][j][0] = true;
                dfs(cells, i - 1, j);
                //s.delete(0);
                break;
            }
        case 1:
            if (j + 1 < n && !cells[i][j + 1])
            {
                maze[i][j + 1][3] = true;
                maze[i][j][1] = true;
                dfs(cells, i, j + 1);
                //s.erase(1);
                break;
            }
        case 2:
            if (i + 1 < n && !cells[i + 1][j])
            {
                maze[i + 1][j][0] = true;
                maze[i][j][2] = true;
                dfs(cells, i + 1, j);
                //s.erase(2);
                break;
            }
        case 3:
            if (j - 1 >= 0 && !cells[i][j - 1])
            {
                maze[i][j - 1][1] = true;
                maze[i][j][3] = true;
                dfs(cells, i, j - 1);
                //s.erase(3);
                break;
            }
        }
        // for (auto it = s.begin(); it !=
        //                      s.end(); ++it)
        // std::cout << ' ' << *it;

        // std::cout<<std::endl;
    }
}

void Maze::show(SDL_Renderer *renderer, SDL_Window *window)
{

    backgroundImage(renderer, grass);

    float ww = cell_size;
    float wh = 2 * cell_size / 5;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < 4; k++)
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
                else if (k == 2)
                {
                    imageCenter(renderer, wall, NULL, ox + cell_size * j + cell_size / 2, oy + cell_size * (i + 1), 
                        ww, wh, 0, SDL_FLIP_NONE);
                }
                else if (k == 3)
                {
                    imageCenter(renderer, wall, NULL, ox + cell_size * j , oy + cell_size * i + cell_size / 2, 
                        ww, wh, 270, SDL_FLIP_NONE);
                } 
            }
        }
    }
}
