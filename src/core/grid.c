#include "grid.h"

void grid_init(Grid *grid)
{
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            grid->cells[y][x] = 0;
            grid->hasMerged[y][x] = 0;
        }
    }
};

void grid_resetMerge(Grid *grid)
{
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            grid->hasMerged[y][x] = 0;
        }
    }
}

uint8_t grid_pushUp(Grid *grid)
{
    uint8_t changeFlag = 0;
    for (int y = 1; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (grid->cells[y - 1][x] == 0 && grid->cells[y][x] != 0)
            {
                // move up by one cell
                grid->cells[y - 1][x] = grid->cells[y][x];
                grid->cells[y][x] = 0;
                changeFlag = 1;
            }
            else if (grid->cells[y - 1][x] != 0 && grid->cells[y - 1][x] == grid->cells[y][x] && !grid->hasMerged[y - 1][x])
            {
                // same value, double upper cell
                grid->cells[y - 1][x] *= 2;
                grid->hasMerged[y - 1][x] = 1;
                grid->cells[y][x] = 0;
                changeFlag = 1;
            }
            else
            {
                continue;
            }
        }
    }
    return changeFlag;
}

uint8_t grid_pushDown(Grid *grid)
{
    uint8_t changeFlag = 0;
    for (int y = 2; y >= 0; y--)
    {
        for (int x = 0; x < 4; x++)
        {
            if (grid->cells[y + 1][x] == 0 && grid->cells[y][x] != 0)
            {
                // move up by one cell
                grid->cells[y + 1][x] = grid->cells[y][x];
                grid->cells[y][x] = 0;
                changeFlag = 1;
            }
            else if (grid->cells[y + 1][x] != 0 && grid->cells[y + 1][x] == grid->cells[y][x] && !grid->hasMerged[y + 1][x])
            {
                // same value, double upper cell
                grid->cells[y + 1][x] *= 2;
                grid->hasMerged[y + 1][x] = 1;
                grid->cells[y][x] = 0;
                changeFlag = 1;
            }
            else
            {
                continue;
            }
        }
    }
    return changeFlag;
}

uint8_t grid_pushLeft(Grid *grid)
{
    uint8_t changeFlag = 0;
    for (int x = 1; x < 4; x++)
    {
        for (int y = 0; y < 4; y++)
        {
            if (grid->cells[y][x - 1] == 0 && grid->cells[y][x] != 0)
            {
                // move up by one cell
                grid->cells[y][x - 1] = grid->cells[y][x];
                grid->cells[y][x] = 0;
                changeFlag = 1;
            }
            else if (grid->cells[y][x - 1] != 0 && grid->cells[y][x - 1] == grid->cells[y][x] && !grid->hasMerged[y][x - 1])
            {
                // same value, double upper cell
                grid->cells[y][x - 1] *= 2;
                grid->hasMerged[y][x - 1] = 1;
                grid->cells[y][x] = 0;
                changeFlag = 1;
            }
            else
            {
                continue;
            }
        }
    }
    return changeFlag;
}

uint8_t grid_pushRight(Grid *grid)
{
    uint8_t changeFlag = 0;
    for (int x = 2; x >= 0; x--)
    {
        for (int y = 0; y < 4; y++)
        {
            if (grid->cells[y][x + 1] == 0 && grid->cells[y][x] != 0)
            {
                // move up by one cell
                grid->cells[y][x + 1] = grid->cells[y][x];
                grid->cells[y][x] = 0;
                changeFlag = 1;
            }
            else if (grid->cells[y][x + 1] != 0 && grid->cells[y][x + 1] == grid->cells[y][x] && !grid->hasMerged[y][x + 1] && !grid->hasMerged[y][x])
            {
                // same value, double upper cell
                grid->cells[y][x + 1] *= 2;
                grid->hasMerged[y][x + 1] = 1;
                grid->cells[y][x] = 0;
                changeFlag = 1;
            }
            else
            {
                continue;
            }
        }
    }
    return changeFlag;
}

void grid_push(Grid *grid, Direction dir)
{
    grid_resetMerge(grid);

    switch (dir)
    {
    case UP:
        while (grid_pushUp(grid))
        {
        }
        break;
    case DOWN:
        while (grid_pushDown(grid))
        {
        }
        break;
    case LEFT:
        while (grid_pushLeft(grid))
        {
        }
        break;
    case RIGHT:
        while (grid_pushRight(grid))
        {
        }
        break;
    }
}