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

void grid_init(Grid *grid, uint8_t gridSize)
{
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

void grid_move(Grid *grid, MoveFrame *frame, MoveDirection dir)
{
    moveFrame_clear(frame);

    // get direction Vectors (in which direction should each cell look at its neighbour?)
    int8_t dx = directions[dir].dx;
    int8_t dy = directions[dir].dy;

    // calc start and end positions and step size for each direction
    uint8_t startX = dx > 0 ? (grid->size - 2) : dx < 0 ? 1
                                                        : 0;
    int8_t endX = dx > 0 ? -1 : dx < 0 ? grid->size
                                       : grid->size; // not actually the end, just the point we dont want to reach
    int8_t stepX = dx > 0 ? -1 : dx < 0 ? 1
                                        : 1;

    uint8_t startY = dy > 0 ? (grid->size - 2) : dy < 0 ? 1
                                                        : 0;
    int8_t endY = dy > 0 ? -1 : dy < 0 ? grid->size
                                       : grid->size; // not actually the end, just the point we dont want to reach
    int8_t stepY = dy > 0 ? -1 : dy < 0 ? 1
                                        : 1;

    uint8_t moved = 0;
    uint8_t round = 0; // needed for starting the while loop (and calculating the origin 

    // interate rounds - a move consists of several consecutive sub-moves
    // only continue if there has been a change in this round
    while (moved || (round == 0))
    {
        moved = 0;
        round += 1;

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
                    // NEW CODE
                    // check if this tile is coming from somewhere
                    uint8_t originX = x + (round - 1) * - dx; // this might overflow in some cases?
                    uint8_t originY = y + (round - 1) * - dy;
                    TileMove *tileMoveOrigin = &frame->moves[originY][originX];
                    tileMoveOrigin->value = grid->cells[y][x];
                    
                    tileMoveOrigin->dx = round * dx;
                    tileMoveOrigin->dy = round * dy;
                    
                    // CORE CODE
                    grid->cells[ny][nx] = grid->cells[y][x];
                    grid->aux[ny][nx] = grid->aux[y][x];

                    grid->cells[y][x] = 0;
                    grid->aux[y][x] = 0;

                    // OLD CODE
                    frame->cells[ny][nx] = grid->cells[ny][nx];
                    frame->cells[y][x] = 0; // for diff rendering
                    frame->moveActive = dir;
                    
                    moved = 1;
                }
                else if (grid->cells[ny][nx] != 0 && grid->cells[ny][nx] == grid->cells[y][x] && !grid->aux[ny][nx] && !grid->aux[y][x])
                {
                    // if the neighbouring cell isnt empty AND neighbouring cell is same value as this cell
                    // AND none of the cell have already merged this turn, neighbour cell += 1

                    // NEW CODE
                    uint8_t originX = x + (round - 1) * - dx; // this might overflow in some cases?
                    uint8_t originY = y + (round - 1) * - dy;
                    TileMove *tileMoveOrigin = &frame->moves[originY][originX];
                    tileMoveOrigin->value = grid->cells[y][x];
                    
                    tileMoveOrigin->dx = round * dx;
                    tileMoveOrigin->dy = round * dy;
                    tileMoveOrigin->merge = 0xF0;
                    frame->moves[nx][ny].merge = 0x0F;

                    // CORE CODE
                    grid->cells[ny][nx] += 1;
                    grid->aux[ny][nx] = 1;
                    grid->cells[y][x] = 0;

                    frame->cells[ny][nx] = grid->cells[ny][nx];
                    frame->cells[y][x] = 0; // for diff rendering
                    frame->moveActive = dir;

                    moved = 1;
                }
                else if (grid->cells[y][x] != 0 && frame->moves[y][x].dx == 0 && frame->moves[y][x].dy == 0 && round == 1) {
                    frame->moves[y][x].value = grid->cells[y][x];
                }

                if (grid->cells[ny][nx] != 0 && frame->moves[ny][nx].dx == 0 && frame->moves[ny][nx].dy == 0 && round == 1) {
                    frame->moves[ny][nx].value = grid->cells[ny][nx];
                }

                x += stepX;
            }
            y += stepY;
        }
        
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

void grid_newCell(Grid *grid, MoveFrame *frame, const uint8_t numNewCells)
{
    // reset the grid.aux[]
    grid_resetAux(grid);

    moveFrame_clear(frame);

    for (uint8_t i = 0; i < numNewCells; i++)
    {

        while (grid_sumAux(grid) < (grid->size * grid->size))
        {
            uint8_t x = (uint8_t)(random_byte() % grid->size);
            uint8_t y = (uint8_t)(random_byte() % grid->size);

            if (grid->cells[y][x] == 0)
            {
                grid->cells[y][x] = (uint8_t)((random_byte() % 2) + 1);
                frame->cells[y][x] = grid->cells[y][x];

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