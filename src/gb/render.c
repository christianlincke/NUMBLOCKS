#include "render.h"

#include <gb/gb.h>

typedef struct
{
    uint8_t TileIndexs[4];
} Tile;

void getTileAdresses(Tile *tile, uint8_t number)
{
    uint8_t startPos = number * 4;
    for (uint8_t i = 0; i < 4; i++)
    {
        tile->TileIndexs[i] = startPos + i;
    }
}

uint8_t *getBkgPos(uint8_t gridX, uint8_t gridY)
{
    uint8_t x = (gridX * 2) + 6;
    uint8_t y = (gridY * 2) + 5;
    return get_bkg_xy_addr(x, y);
}

void renderGrid(Grid *grid)
{
    Tile tile;
    for (int y = 0; y < GRID_SIZE; y++)
    {
        for (int x = 0; x < GRID_SIZE; x++)
        {
            getTileAdresses(&tile, grid->cells[y][x]);

            uint8_t bx = x * 2 + 6;
            uint8_t by = y * 2 + 5;

            set_vram_byte(get_bkg_xy_addr(bx, by), tile.TileIndexs[0]);
            set_vram_byte(get_bkg_xy_addr(bx + 1, by), tile.TileIndexs[2]);
            set_vram_byte(get_bkg_xy_addr(bx, by + 1), tile.TileIndexs[1]);
            set_vram_byte(get_bkg_xy_addr(bx + 1, by + 1), tile.TileIndexs[3]);
        }
    }
}