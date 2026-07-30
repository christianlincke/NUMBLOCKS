#include "render.h"

#include <gb/gb.h>

// ASSETS
#include "tiles.h"
#include "tiles_empty.h"
#include "gameover.h"
#include "start_screen.h"

#define GAME_OVER_ANIMATION_SPEED 30 // vsyncs per tile

void rendererBegin(Renderer *renderer, uint8_t gridSize)
{
    renderer->size = gridSize;
    
    // load tiles, empties and font
    set_bkg_data(0, tiles_TILE_COUNT, tiles_tiles);
    set_bkg_data(tiles_TILE_COUNT, gameover_TILE_COUNT, gameover_tiles);
    set_bkg_data(tiles_TILE_COUNT + gameover_TILE_COUNT, tiles_empty_TILE_COUNT, tiles_empty_tiles);

    // fill screen
    fill_bkg_rect(0, 0, DEVICE_SCREEN_WIDTH, DEVICE_SCREEN_HEIGHT, tiles_TILE_COUNT + gameover_TILE_COUNT);
    SHOW_BKG;

    for (int y = 0; y < renderer->size; y++)
    {
        for (int x = 0; x < renderer->size; x++)
        {
            renderer->frame[y][x] = 0;
        }
    }
}

void renderGrid(Renderer *renderer, MoveFrame *frame)
{
    for (int y = 0; y < renderer->size; y++)
    {
        for (int x = 0; x < renderer->size; x++)
        {
            // only render if theres a change
            if (frame->cells[y][x] >= 0)
            {
                renderer->frame[y][x] = frame->cells[y][x];
            }

            uint8_t map[] = {4 * renderer->frame[y][x], 4 * renderer->frame[y][x] + 2, 4 * renderer->frame[y][x] + 1, 4 * renderer->frame[y][x] + 3};

            uint8_t bx = x * 2 + (20 - 2 * renderer->size) / 2; // SCREENWIDTH in gb tiles
            uint8_t by = y * 2 + (18 - 2 * renderer->size) / 2; // SCREENHEIGHT in gb tiles

            set_bkg_tiles(bx, by, 2, 2, map);
        }
    }
}

void renderScore(uint16_t score)
{
}

void renderGameOverAnimation(Renderer *renderer)
{
    uint8_t startTile;
    uint8_t counter;
    uint8_t tileMap0[4] = {0, 2, 1, 3};

    // first line all empty
    for (int y = 0; y < renderer->size; y++)
    {
        for (int x = 0; x < renderer->size; x++)
        {
            uint8_t bx = x * 2 + ((20 - 2 * renderer->size) / 2); // SCREENWIDTH in gb tiles
            uint8_t by = y * 2 + ((18 - 2 * renderer->size) / 2); // SCREENHEIGHT in gb tiles

            if (y == 1) {
                startTile = tiles_TILE_COUNT;
                uint8_t tileMap[] = {x * 4 + startTile + 0, x * 4 + startTile + 2, x * 4 + startTile + 1, x * 4 + startTile + 3};
                set_bkg_tiles(bx, by, 2, 2, tileMap);
            }
            else if (y == 2) {
                startTile = (uint8_t)tiles_TILE_COUNT + (gameover_TILE_COUNT / 2);
                uint8_t tileMap[] = {x * 4 + startTile + 0, x * 4 + startTile + 2, x * 4 + startTile + 1, x * 4 + startTile + 3};
                set_bkg_tiles(bx, by, 2, 2, tileMap);
            }
            else {
                uint8_t tileMap0[] = {0, 2, 1, 3};
                set_bkg_tiles(bx, by, 2, 2, tileMap0);
            }

            counter = 0;
            while (counter < GAME_OVER_ANIMATION_SPEED)
            {
                counter++;
                vsync();
            }
        }
    }
}

void renderStartScreen()
{
    // Load Background tiles and then map
    set_bkg_data(0, start_screen_TILE_COUNT, start_screen_tiles);
    set_bkg_tiles(0, 0, 20, 18, start_screen_map);
}
