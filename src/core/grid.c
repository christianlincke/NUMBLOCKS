#include "grid.h"

#include "random_byte.h"
#include <string.h>

typedef struct
{
    int8_t dx;
    int8_t dy;
} DirectionVector;

static const DirectionVector directions[] = {
    [MOVE_UP] = {0, -1}, [MOVE_DOWN] = {0, 1}, [MOVE_LEFT] = {-1, 0}, [MOVE_RIGHT] = {1, 0}};

void grid_init(Grid *grid, MoveFrame *frame, uint8_t gridSize)
{
    grid->frame = frame;
    grid->size = gridSize;
    for (int y = 0; y < grid->size; y++)
    {
        for (int x = 0; x < grid->size; x++)
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
    for (int y = 0; y < grid->size; y++)
    {
        for (int x = 0; x < grid->size; x++)
        {
            grid->aux[y][x] = 0;
        }
    }
}

void grid_prepare(Grid *grid)
{
    grid_resetAux(grid);
}

void grid_move(Grid *grid)
{
    MoveDirection dir = grid->frame->moveActive;
    // get direction Vectors (in which direction should each cell look at its neighbour?)
    int8_t dx = directions[dir].dx;
    int8_t dy = directions[dir].dy;

    // clear moveFrame
    moveFrame_clear(grid->frame);

    // calc start and end positions and step size for each direction
    uint8_t startX = dx > 0 ? (grid->size - 2) : dx < 0 ? 1 : 0;
    int8_t endX = dx > 0 ? -1 : dx < 0 ? grid->size : grid->size;
    int8_t stepX = dx > 0 ? -1 : dx < 0 ? 1 : 1;

    uint8_t startY = dy > 0 ? (grid->size - 2) : dy < 0 ? 1 : 0;
    int8_t endY = dy > 0 ? -1 : dy < 0 ? grid->size : grid->size;
    int8_t stepY = dy > 0 ? -1 : dy < 0 ? 1 : 1;

    uint8_t moved = 0;

    int8_t y = startY;
    while (y != endY)
    {
        int8_t x = startX;
        while (x != endX)
        {
            // call neighbouring cell coordinates
            uint8_t nx = x + dx;
            uint8_t ny = y + dy;

            if (grid->cells[ny][nx] == 0 && grid->cells[y][x] != 0)
            {
                // CORE CODE
                grid->cells[ny][nx] = grid->cells[y][x];
                grid->aux[ny][nx] = grid->aux[y][x];

                grid->cells[y][x] = 0;
                grid->aux[y][x] = 0;

                // NEW CODE
                grid->frame->moves[y][x] |= (dx >> 6) | dx;
                grid->frame->moves[y][x] |= (dy >> 4) | (dy << 2);
                grid->frame->moves[y][x] &= 0x0F;
                grid->frame->moveActive = dir;

                moved = 1;
            }
            else if (grid->cells[ny][nx] != 0 && grid->cells[ny][nx] == grid->cells[y][x] &&
                     !grid->aux[ny][nx] && !grid->aux[y][x])
            {
                // if the neighbouring cell isnt empty AND neighbouring cell is same value as
                // this cell AND none of the cell have already merged this turn, neighbour cell
                // += 1

                // CORE CODE
                grid->cells[ny][nx] += 1;
                grid->aux[ny][nx] = 1;
                grid->cells[y][x] = 0;

                // NEW CODE
                grid->frame->moves[y][x] &= 0xCF;
                grid->frame->moves[nx][ny] &= 0x3F;
                
                grid->frame->moves[y][x] |= (dx >> 6) | dx;
                grid->frame->moves[y][x] |= (dy >> 4) | (dy << 2);
                grid->frame->moveActive = dir;

                moved = 1;
            }
            x += stepX;
        }
        y += stepY;
    }
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
    for (uint8_t y = 0; y < grid->size; y++)
    {
        for (uint8_t x = 0; x < grid->size; x++)
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
    for (uint8_t y = 0; y < grid->size; y++)
    {
        for (uint8_t x = 0; x < grid->size; x++)
        {
            sum += grid->aux[y][x];
        }
    }
    return sum;
}

void grid_newCell(Grid *grid, const uint8_t numNewCells)
{
    // reset the grid.aux[]
    grid_resetAux(grid);

    for (uint8_t i = 0; i < numNewCells; i++)
    {

        while (grid_sumAux(grid) < (grid->size * grid->size))
        {
            uint8_t x = (uint8_t)(random_byte() % grid->size);
            uint8_t y = (uint8_t)(random_byte() % grid->size);

            if (grid->cells[y][x] == 0)
            {
                grid->cells[y][x] = (uint8_t)((random_byte() % 2) + 1);
                break;
            }
            grid->aux[y][x] = 1;
        }
    }
    grid_resetAux(grid);
}

uint8_t grid_checkGameOver(Grid *grid)
{

    for (uint8_t y = 0; y < grid->size; y++)
    {
        for (uint8_t x = 0; x < grid->size; x++)
        {
            if (grid->cells[y][x] == 0)
            {
                return 0;
            }

            if (x < grid->size - 1)
            {
                if (grid->cells[y][x] == grid->cells[y][x + 1])
                {
                    return 0;
                }
            }

            if (y < grid->size - 1)
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