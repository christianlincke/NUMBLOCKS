#include "render.h"

#include <gb/gb.h>

void rendererBegin(Renderer *renderer)
{
    for (int y = 0; y < GRID_SIZE; y++)
    {
        for (int x = 0; x < GRID_SIZE; x++)
        {
            renderer->frame[y][x] = 0;

            uint8_t map[] = {4 * renderer->frame[y][x], 4 * renderer->frame[y][x] + 2, 4 * renderer->frame[y][x] + 1, 4 * renderer->frame[y][x] + 3};

            uint8_t bx = x * 2 + 6;
            uint8_t by = y * 2 + 5;

            set_bkg_tiles(bx, by, 2, 2, map);
        }
    }
}

void renderGrid(Renderer *renderer, MoveFrame *frame)
{
    for (int y = 0; y < GRID_SIZE; y++)
    {
        for (int x = 0; x < GRID_SIZE; x++)
        {
            // only render if theres a change
            if (frame->cells[y][x] >= 0)
            {
                renderer->frame[y][x] = frame->cells[y][x];
            }

            uint8_t map[] = {4 * renderer->frame[y][x], 4 * renderer->frame[y][x] + 2, 4 * renderer->frame[y][x] + 1, 4 * renderer->frame[y][x] + 3};

            uint8_t bx = x * 2 + 6;
            uint8_t by = y * 2 + 5;

            set_bkg_tiles(bx, by, 2, 2, map);
        }
    }
}


void renderScore(uint16_t score) {
}

