#include "grid.h"

#include "random_byte.h"

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
}

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
            else if (grid->cells[y - 1][x] != 0 && grid->cells[y - 1][x] == grid->cells[y][x] && !grid->hasMerged[y - 1][x] && !grid->hasMerged[y][x])
            {
                // same value, double upper cell
                grid->cells[y - 1][x] += 1;
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
            else if (grid->cells[y + 1][x] != 0 && grid->cells[y + 1][x] == grid->cells[y][x] && !grid->hasMerged[y + 1][x] && !grid->hasMerged[y][x])
            {
                // same value, double upper cell
                grid->cells[y + 1][x] += 1;
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
            else if (grid->cells[y][x - 1] != 0 && grid->cells[y][x - 1] == grid->cells[y][x] && !grid->hasMerged[y][x - 1] && !grid->hasMerged[y][x])
            {
                // same value, double upper cell
                grid->cells[y][x - 1] += 1;
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
                grid->cells[y][x + 1] += 1;
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
    case GRID_UP:
        while (grid_pushUp(grid))
        {
        }
        break;
    case GRID_DOWN:
        while (grid_pushDown(grid))
        {
        }
        break;
    case GRID_LEFT:
        while (grid_pushLeft(grid))
        {
        }
        break;
    case GRID_RIGHT:
        while (grid_pushRight(grid))
        {
        }
        break;
    }
}

/**
 * @brief adds an randomly selected empty sell with a random value (2 or 4)
 *
 * @param grid the grid
 */
void grid_newCell(Grid *grid)
{
    uint8_t busy = 1;

    uint8_t attempts = 0;

    while (busy && attempts < 100)
    {
        uint8_t x = (uint8_t)(random_byte() % 4);
        uint8_t y = (uint8_t)(random_byte() % 4);

        if (grid->cells[y][x] == 0)
        {
            grid->cells[y][x] = (uint8_t)((random_byte() % 2) + 1);
            busy = 0;
        }

        attempts++;
    }
}

/**
 * @brief dump grid content into textbuffer
 *
 * @param grid
 * @param buffer
 */
void grid_dump(Grid *grid, TextBuffer *buffer)
{
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            buffer->text[y + 7][x + 8] = '0' + grid->cells[y][x];
        }
    }
}