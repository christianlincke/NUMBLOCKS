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

void rendererBegin(Renderer *renderer)
{
    Tile tile;
    for (int y = 0; y < GRID_SIZE; y++)
    {
        for (int x = 0; x < GRID_SIZE; x++)
        {
            renderer->frame[y][x] = 0;
            getTileAdresses(&tile, 0);

            uint8_t bx = x * 2 + 6;
            uint8_t by = y * 2 + 5;

            set_vram_byte(get_bkg_xy_addr(bx, by), tile.TileIndexs[0]);
            set_vram_byte(get_bkg_xy_addr(bx + 1, by), tile.TileIndexs[2]);
            set_vram_byte(get_bkg_xy_addr(bx, by + 1), tile.TileIndexs[1]);
            set_vram_byte(get_bkg_xy_addr(bx + 1, by + 1), tile.TileIndexs[3]);
            
            
        }
    }
}

void renderGrid(Renderer *renderer, MoveFrame *frame)
{
    Tile tile;
    for (int y = 0; y < GRID_SIZE; y++)
    {
        for (int x = 0; x < GRID_SIZE; x++)
        {
            // only render if theres a change
            if (frame->cells[y][x] >= 0){
                renderer->frame[y][x] = frame->cells[y][x];
            }

            getTileAdresses(&tile, renderer->frame[y][x]);

            uint8_t bx = x * 2 + 6;
            uint8_t by = y * 2 + 5;

            set_vram_byte(get_bkg_xy_addr(bx, by), tile.TileIndexs[0]);
            set_vram_byte(get_bkg_xy_addr(bx + 1, by), tile.TileIndexs[2]);
            set_vram_byte(get_bkg_xy_addr(bx, by + 1), tile.TileIndexs[1]);
            set_vram_byte(get_bkg_xy_addr(bx + 1, by + 1), tile.TileIndexs[3]);
            
        }
    }
}