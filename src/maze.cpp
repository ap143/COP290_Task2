#include "game.hpp"
#include <algorithm>

Maze::Maze()
{
    maze = new bool **[n];
    for (int i = 0; i < n; i++)
    {
        maze[i] = new bool *[n];
        for (int j = 0; j < n; j++)
        {
            maze[i][j] = new bool[4]();
        }
    }
}

Maze::Maze(int size)
{
    n = size;
    maze = new bool **[n];
    for (int i = 0; i < n; i++)
    {
        maze[i] = new bool *[n];
        for (int j = 0; j < n; j++)
        {
            maze[i][j] = new bool[4];
        }
    }
}

void Maze::generate()
{
    std::vector<std::vector<bool>> cells(n, std::vector<bool>(n, false));
    srand(time(0));
    dfs(cells, 0, 0);
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

    for (int k = 0; k < s.size(); k++)
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

    int w = 1200;
    int h = 1000;

    //SDL_GetWindowSize(window, w, h);

    float cell_size = (float)(19*(h)/20)/n;

    float grid_length = (float)19*(h)/20;

    rect(renderer,(w)/2- grid_length/2, (h)/2- grid_length/2, grid_length, grid_length, 0);
    
    float ox =(float) (w)/2- (float)grid_length/2;
    float oy = (float)(h)/2- (float)grid_length/2;
    
    
    for (int i = 0;i<n; i++){
        for (int j = 0; j<n; j++) {
            for (int k = 0; k<4; k++) {
                // std::cout<<maze[i][j][k]<<std::endl;
                // std::cout<<i<<" "<<j<<" "<<k<<std::endl;
                if (!maze[i][j][k])
                {
                    if (k == 0)
                    {
                        line(renderer, ox + cell_size * j, oy + cell_size * i, ox + cell_size * (j + 1), oy + cell_size * i);
                    }
                    else if (k == 1)
                    {
                        line(renderer, ox + cell_size * (j + 1), oy + cell_size * i, ox + cell_size * (j + 1), oy + cell_size * (i + 1));
                    }
                    else if (k == 2)
                    {
                        line(renderer, ox + cell_size * j, oy + cell_size * (i + 1), ox + cell_size * (j + 1), oy + cell_size * (i + 1));
                    }
                    else
                    {
                        line(renderer, ox + cell_size * j, oy + cell_size * (i + 1), ox + cell_size * j, oy + cell_size * i);
                    }
                }
            }
        }
    }
}
