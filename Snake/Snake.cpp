#include <iostream>
#include <vector>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include <thread>
using namespace std;

enum direction
{
    left,
    right,
    up,
    down,
    stop
};

int n = 10;
string field[15][15];
direction directs[15][15];
vector<pair<int, int>> snakeParts;
int x, y;
int tx, ty;
const int fps = 60;
const int sleep = 1000 / fps;
bool playing = true;

bool inField(int x, int y)
{
    return x >= 1 && y >= 1 && x <= n && y <= n;
}

void print()
{
    {
        field[tx][ty] = "$";
        system("cls");
        for (int i = 1; i <= n; ++i)
        {
            for (int j = 1; j <= n; ++j)
            {
                printf("%s", field[i][j].data());
            }
            printf("\n");
        }
    }
}

void move(bool con)
{
    while (playing)
    {
        this_thread::sleep_for(chrono::milliseconds(sleep));
        int len = snakeParts.size();
        if (field[x][y] == "*")
        {
            playing = false;
        }
        if (tx == x && ty == y)
        {
            int lx = snakeParts.back().first, ly = snakeParts.back().second;
            switch (directs[lx][ly])
            {
                case direction::left:
                    ++ly;
                    break;
                case direction::right:
                    --ly;
                    break;
                case direction::up:
                    ++lx;
                    break;
                case direction::down:
                    --lx;
                    break;
            }
            snakeParts.emplace_back(lx, ly);
            tx = rand() % n + 1;
            ty = rand() % n + 1;
            field[tx][ty] = "$";
        }
        for (int i = 0; i < len; ++i)
        {
            int& sx = snakeParts[i].first;
            int& sy = snakeParts[i].second;
            field[sx][sy] = " ";
            switch (directs[sx][sy])
            {
                case direction::up:
                    if (!i)
                    {
                        directs[sx - 1][sy] = directs[sx][sy];
                    }
                    --sx;
                    break;
                case direction::down:
                    if (!i)
                    {
                        directs[sx + 1][sy] = directs[sx][sy];
                    }
                    ++sx;
                    break;
                case direction::right:
                    if (!i)
                    {
                        directs[sx][sy + 1] = directs[sx][sy];
                    }
                    ++sy;
                    break;
                case direction::left:
                    if (!i)
                    {
                        directs[sx][sy - 1] = directs[sx][sy];
                    }
                    --sy;
                    break;
            }
            if (!inField(sx, sy))
            {
                if (!sx)
                {
                    if (!i)
                    {
                        directs[n][sy] = directs[sx][sy];
                    }
                    sx = n;
                }
                else if (sx > n)
                {
                    if (!i)
                    {
                        directs[1][sy] = directs[sx][sy];
                    }
                    sx = 1;
                }
                else if (!sy)
                {
                    if (!i)
                    {
                        directs[sx][n] = directs[sx][sy];
                    }
                    sy = n;
                }
                else
                {
                    if (!i)
                    {
                        directs[sx][1] = directs[sx][sy];
                    }
                    sy = 1;
                }
            }
            if (i)
            {
                field[sx][sy] = "*";
            }
            else
            {
                x = sx, y = sy;
                field[sx][sy] = "@";
            }
        }
        if (con)
        {
            print();
        }
    }
}

void selectDirection()
{
    while (playing)
    {
        this_thread::sleep_for(chrono::milliseconds(sleep));
        if (_kbhit())
        {
            if (GetAsyncKeyState('W'))
            {
                if (directs[x][y] != direction::down)
                {
                    directs[x][y] = direction::up;
                }
            }
            else if (GetAsyncKeyState('A'))
            {
                if (directs[x][y] != direction::right)
                {
                    directs[x][y] = direction::left;
                }
            }
            else if (GetAsyncKeyState('S'))
            {
                if (directs[x][y] != direction::up)
                {
                    directs[x][y] = direction::down;
                }
            }
            else if (GetAsyncKeyState('D'))
            {
                if (directs[x][y] != direction::left)
                {
                    directs[x][y] = direction::right;
                }
            }
        }
    }
}

int main()
{
    srand(time(nullptr));
    x = n / 2, y = n / 2;
    tx = rand() % n + 1;
    ty = rand() % n + 1;
    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= n; ++j)
        {
            directs[i][j] = direction::stop;
            field[i][j] = " ";
        }
    }
    field[tx][ty] = "$";
    field[x][y] = "@";
    snakeParts.emplace_back(x, y);
    thread th(selectDirection);
    move(true);
    th.join();
    return 0;
}
