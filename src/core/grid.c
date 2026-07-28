#include "grid.h"

#include "random_byte.h"

typedef struct
{
    int8_t dx;
    int8_t dy;
} DirectionVector;

static const DirectionVector directions[] = {
    [GRID_UP] = {0, -1},
    [GRID_DOWN] = {0, 1},
    [GRID_LEFT] = {-1, 0},
    [GRID_RIGHT] = {1, 0}};

void grid_init(Grid *grid)
{
    for (int y = 0; y < GRID_SIZE; y++)
    {
        for (int x = 0; x < GRID_SIZE; x++)
        {
            grid->cells[y][x] = 0;
            grid->hasMerged[y][x] = 0;
        }
    }
}

void grid_resetMerge(Grid *grid)
{
    for (int y = 0; y < GRID_SIZE; y++)
    {
        for (int x = 0; x < GRID_SIZE; x++)
        {
            grid->hasMerged[y][x] = 0;
        }
    }
}

uint8_t grid_pushUp(Grid *grid, PushDirection dir)
{
    // get direction Vectors (in which direction should each cell look at its neighbour?)
    int8_t dx = directions[dir].dx;
    int8_t dy = directions[dir].dy;

    // calc start and end positions and step size for each direction
    uint8_t startX = dx > 0 ? 2 : dx < 0 ? 1
                                         : 0;
    int8_t endX = dx > 0 ? -1 : dx < 0 ? GRID_SIZE
                                       : GRID_SIZE; // not actually the end, just the point we dont want to reach
    int8_t stepX = dx > 0 ? -1 : dx < 0 ? 1
                                        : 1;

    uint8_t startY = dy > 0 ? 2 : dy < 0 ? 1
                                         : 0;
    int8_t endY = dy > 0 ? -1 : dy < 0 ? GRID_SIZE
                                       : GRID_SIZE; // not actually the end, just the point we dont want to reach
    int8_t stepY = dy > 0 ? -1 : dy < 0 ? 1
                                        : 1;

    uint8_t changeFlag = 0;

    int y = startY;
    while (y != endY)
    {
        int x = startX;
        while (x != endX)
        {
            uint8_t nx = x + dx;
            uint8_t ny = y + dy;
            if (grid->cells[ny][nx] == 0 && grid->cells[y][x] != 0)
            {
                // move up by one cell
                grid->cells[ny][nx] = grid->cells[y][x];
                grid->cells[y][x] = 0;
                changeFlag = 1;
            }
            else if (grid->cells[ny][nx] != 0 && grid->cells[ny][nx] == grid->cells[y][x] && !grid->hasMerged[ny][nx] && !grid->hasMerged[y][x])
            {
                // same value, neighbour cell += 1
                grid->cells[ny][nx] += 1;
                grid->hasMerged[ny][nx] = 1;
                grid->cells[y][x] = 0;
                changeFlag = 1;
            }
            x += stepX;
        }
        y += stepY;
    }
    return changeFlag;
}

void grid_push(Grid *grid, PushDirection dir)
{
    grid_resetMerge(grid);

    // TODO move the actual code here maybe
    while (grid_pushUp(grid, dir))
    {
    }
}

/**
 * @brief adds an randomly selected empty sell with a random value (2 or 4)
 *
 * @param grid the grid
 */
void grid_newCell(Grid *grid)
{
    // TODO this should be done a different way, not sure how. maybe generate rnd starting point and then iterate systematically
    uint8_t busy = 1;

    uint8_t attempts = 0;

    while (busy && attempts < 100)
    {
        uint8_t x = (uint8_t)(random_byte() % GRID_SIZE);
        uint8_t y = (uint8_t)(random_byte() % GRID_SIZE);

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
    uint8_t bStartX = (uint8_t)(20 - GRID_SIZE) / 2;
    uint8_t bStartY = (uint8_t)(18 - GRID_SIZE) / 2;

    for (int y = 0; y < GRID_SIZE; y++)
    {
        for (int x = 0; x < GRID_SIZE; x++)
        {
            buffer->text[y + bStartY][x + bStartX] = '0' + grid->cells[y][x];
        }
    }
}