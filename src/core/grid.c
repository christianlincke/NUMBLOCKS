#include "grid.h"

#include "random_byte.h"
#include <string.h>

typedef struct
{
    int8_t dx;
    int8_t dy;
} DirectionVector;

static const DirectionVector directions[] = {
    [MOVE_UP] = {0, -1},
    [MOVE_DOWN] = {0, 1},
    [MOVE_LEFT] = {-1, 0},
    [MOVE_RIGHT] = {1, 0}};

void grid_init(Grid *grid)
{
    for (int y = 0; y < GRID_SIZE; y++)
    {
        for (int x = 0; x < GRID_SIZE; x++)
        {
            grid->cells[y][x] = 0;
            grid->aux[y][x] = 0;
        }
    }
}

/**
 * @brief reset the aux flag
 *
 * @param grid
 */
void grid_resetAux(Grid *grid)
{
    for (int y = 0; y < GRID_SIZE; y++)
    {
        for (int x = 0; x < GRID_SIZE; x++)
        {
            grid->aux[y][x] = 0;
        }
    }
}

void grid_prepare(Grid *grid)
{
    grid_resetAux(grid);
}

MoveFrame grid_move(Grid *grid, MoveDirection dir)
{
    MoveFrame frame;
    moveFrame_clear(&frame);

    // get direction Vectors (in which direction should each cell look at its neighbour?)
    int8_t dx = directions[dir].dx;
    int8_t dy = directions[dir].dy;

    // calc start and end positions and step size for each direction
    uint8_t startX = dx > 0 ? (GRID_SIZE - 2) : dx < 0 ? 1
                                         : 0;
    int8_t endX = dx > 0 ? -1 : dx < 0 ? GRID_SIZE
                                       : GRID_SIZE; // not actually the end, just the point we dont want to reach
    int8_t stepX = dx > 0 ? -1 : dx < 0 ? 1
                                        : 1;

    uint8_t startY = dy > 0 ? (GRID_SIZE - 2) : dy < 0 ? 1
                                         : 0;
    int8_t endY = dy > 0 ? -1 : dy < 0 ? GRID_SIZE
                                       : GRID_SIZE; // not actually the end, just the point we dont want to reach
    int8_t stepY = dy > 0 ? -1 : dy < 0 ? 1
                                        : 1;

    // uint8_t moved = 0;

    int y = startY;
    while (y != endY)
    {
        int x = startX;
        while (x != endX)
        {
            // call neighbouring cell coordinates
            uint8_t nx = x + dx;
            uint8_t ny = y + dy;

            if (grid->cells[ny][nx] == 0 && grid->cells[y][x] != 0)
            {

                grid->cells[ny][nx] = grid->cells[y][x];
                grid->aux[ny][nx] = grid->aux[y][x];

                grid->cells[y][x] = 0;
                grid->aux[y][x] = 0;

                frame.cells[ny][nx] = grid->cells[ny][nx];
                frame.cells[y][x] = 0; // for diff rendering
                frame.moveActive = dir;

                
            }
            else if (grid->cells[ny][nx] != 0 && grid->cells[ny][nx] == grid->cells[y][x] && !grid->aux[ny][nx] && !grid->aux[y][x])
            {
                // if the neighbouring cell isnt empty AND neighbouring cell is same value as this cell
                // AND none of the cell have already merged this turn, neighbour cell += 1

                grid->cells[ny][nx] += 1;
                grid->aux[ny][nx] = 1;
                grid->cells[y][x] = 0;

                frame.cells[ny][nx] = grid->cells[ny][nx];
                frame.cells[y][x] = 0; // for diff rendering
                frame.moveActive = dir;
            }
            x += stepX;
        }
        y += stepY;
    }

    return frame;
}

/**
 * @brief sum the cells
 *
 * @param grid
 * @return uint8_t
 */
uint16_t grid_sumCells(Grid *grid)
{
    uint16_t sum = 0;
    for (uint8_t y = 0; y < GRID_SIZE; y++)
    {
        for (uint8_t x = 0; x < GRID_SIZE; x++)
        {
            sum += grid->cells[y][x];
        }
    }
    return sum;
}

/**
 * @brief sum the aux cells
 *
 * @param grid
 * @return uint8_t
 */
uint8_t grid_sumAux(Grid *grid)
{
    uint8_t sum = 0;
    for (uint8_t y = 0; y < GRID_SIZE; y++)
    {
        for (uint8_t x = 0; x < GRID_SIZE; x++)
        {
            sum += grid->aux[y][x];
        }
    }
    return sum;
}

MoveFrame grid_newCell(Grid *grid)
{
    // reset the grid.aux[]
    grid_resetAux(grid);

    MoveFrame frame;
    moveFrame_clear(&frame);

    while (grid_sumAux(grid) < (GRID_SIZE * GRID_SIZE))
    {
        uint8_t x = (uint8_t)(random_byte() % GRID_SIZE);
        uint8_t y = (uint8_t)(random_byte() % GRID_SIZE);

        if (grid->cells[y][x] == 0)
        {
            grid->cells[y][x] = (uint8_t)((random_byte() % 2) + 1);
            frame.cells[y][x] = grid->cells[y][x];

            return frame;
        }
        grid->aux[y][x] = 1;
    }
    grid_resetAux(grid);
    return frame;
}

uint8_t grid_checkGameOver(Grid *grid)
{

    for (uint8_t y = 0; y < GRID_SIZE; y++)
    {
        for (uint8_t x = 0; x < GRID_SIZE; x++)
        {

            if (x < 3)
            {
                if (grid->cells[y][x] == grid->cells[y][x + 1])
                {
                    return 0;
                }
            }

            if (y < 3)
            {
                if (grid->cells[y][x] == grid->cells[y + 1][x])
                {
                    return 0;
                }
            }
        }
    }
    return 1;
}