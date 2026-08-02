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


void moveData_clear(MoveData *moveData) {

    moveData->moveDir = MOVE_NONE;

    moveData->dx = 0;
    moveData->startX = 0;
    moveData->endX = 0;
    moveData->stepX = 0;

    moveData->dy = 0;
    moveData->startY = 0;
    moveData->endY = 0;
    moveData->stepY = 0;
}

void grid_init(Grid *grid, MoveFrame *frame, uint8_t gridSize)
{
    grid->frame = frame;
    grid->size = gridSize;
    moveData_clear(&grid->moveData);
    for (int y = 0; y < grid->size; y++)
    {
        for (int x = 0; x < grid->size; x++)
        {
            grid->cells[y][x] = 0;
            grid->aux[y] = 0x00;
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
        grid->aux[y] = 0x00;
    }
}

void grid_prepare(Grid *grid, MoveDirection moveDir)
{
    grid_resetAux(grid);

    grid->moveData.moveDir = moveDir;
    int8_t dx = directions[moveDir].dx;

    grid->moveData.dx = dx;
    grid->moveData.startX = dx > 0 ? (grid->size - 2) : dx < 0 ? 1 : 0;
    grid->moveData.endX = dx > 0 ? -1 : dx < 0 ? grid->size : grid->size;
    grid->moveData.stepX = dx > 0 ? -1 : dx < 0 ? 1 : 1;

    int8_t dy = directions[moveDir].dy;
    grid->moveData.dy = dy;
    grid->moveData.startY = dy > 0 ? (grid->size - 2) : dy < 0 ? 1 : 0;
    grid->moveData.endY = dy > 0 ? -1 : dy < 0 ? grid->size : grid->size;
    grid->moveData.stepY = dy > 0 ? -1 : dy < 0 ? 1 : 1;
}

MoveDirection grid_move(Grid *grid)
{
    // clear moveFrame
    moveFrame_clear(grid->frame);

    uint8_t moved = 0;

    int8_t y = grid->moveData.startY;
    while (y != grid->moveData.endY)
    {
        int8_t x = grid->moveData.startX;
        while (x != grid->moveData.endX)
        {
            // call neighbouring cell coordinates
            uint8_t nx = x + grid->moveData.dx;
            uint8_t ny = y + grid->moveData.dy;

            if (grid->cells[ny][nx] == 0 && grid->cells[y][x] != 0)
            {
                // CORE CODE
                grid->cells[ny][nx] = grid->cells[y][x];

                // grid->aux[ny][nx] = grid->aux[y][x];
                grid->aux[ny] |= (grid->aux[y] & (0x80 >> x)) >> grid->moveData.dx;

                grid->cells[y][x] = 0;
                // grid->aux[y][x] = 0;
                grid->aux[y] &= ~(0x80 >> x);

                // NEW CODE
                grid->frame->moves[y][x] |= (grid->moveData.dx >> 6) | grid->moveData.dx;
                grid->frame->moves[y][x] |= (grid->moveData.dy >> 4) | (grid->moveData.dy << 2);
                grid->frame->moves[y][x] &= 0x0F;
                grid->frame->moveActive = grid->moveData.moveDir;

                moved = 1;
            }
            else if (grid->cells[ny][nx] != 0 && grid->cells[ny][nx] == grid->cells[y][x] &&
                     !(grid->aux[ny] & (0x80 >> nx)) &&
                     !(grid->aux[y] & (0x80 >> x))) // !grid->aux[ny][nx] && !grid->aux[y][x]
            {
                // if the neighbouring cell isnt empty AND neighbouring cell is same value as
                // this cell AND none of the cell have already merged this turn, neighbour cell
                // += 1

                // CORE CODE
                grid->cells[ny][nx] += 1;
                grid->aux[ny] |= 0x80 >> nx; // grid->aux[ny][nx] = 1;
                grid->cells[y][x] = 0;

                // NEW CODE
                grid->frame->moves[y][x] &= 0xCF;
                grid->frame->moves[nx][ny] &= 0x3F;

                grid->frame->moves[y][x] |= (grid->moveData.dx >> 6) | grid->moveData.dx;
                grid->frame->moves[y][x] |= (grid->moveData.dy >> 4) | (grid->moveData.dy << 2);
                grid->frame->moveActive = grid->moveData.moveDir;

                moved = 1;
            }
            x += grid->moveData.stepX;
        }
        y += grid->moveData.stepY;
    }
    if (moved) {
        return grid->moveData.moveDir;
    }
    else {
        return MOVE_NONE;
    }
}

/**
 * @brief sum the cells
 *
 * @param grid
 * @return uint8_t
 */
uint16_t grid_calcScore(const Grid *grid)
{
    uint16_t sum = 0;
    for (uint8_t y = 0; y < grid->size; y++)
    {
        for (uint8_t x = 0; x < grid->size; x++)
        {
            if (grid->cells[y][x] > 0) {
                sum += 1 << grid->cells[y][x];
            }
            
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
            sum += ((grid->aux[y] >> x) & 0xA0) >> 8;
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
            grid->aux[y] |= 0xA0 >> x;
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