#include "render.h"

#include <gb/gb.h>
#include <gbdk/console.h>
#include <gbdk/font.h>

// ASSETS
#include "ibm_minimal_dark.h"
#include "start_screen.h"
#include "tiles.h"
#include "tiles_empty.h"

#include <stdio.h>

#include <stdlib.h>

#define GAME_OVER_ANIMATION_SPEED 30 // vsyncs per tile

uint8_t getTileAddress(const char c)
{
    uint8_t address = 0;

    if (c >= 'a' && c <= 'z')
        address = (c - 'a') + 0x81;
    else if (c >= 'A' && c <= 'Z')
        address = (c - 'A') + 0x81;
    else if (c >= '0' && c <= '9')
        address = (c - '0') + 0x9B;
    else
    {
        switch (c)
        {
        case '!':
            address = 0xA5;
            break;
        case ':':
            address = 0xA6;
            break;
        case '?':
            address = 0xA7;
            break;
        case '>':
            address = 0xA8;
            break;
        case '.':
            address = 0xA9;
            break;
        case ' ':
            address = 0x80;
            break;
        default:
            address = 0xA7;
            break;
        }
    }
    return address;
}

void loadAssets()
{
    // load tiles, empties and font
    set_bkg_data(0x00, tiles_TILE_COUNT, tiles_tiles); // 0x80 - 0x5F
    set_bkg_data(0x60, tiles_empty_TILE_COUNT, tiles_empty_tiles);
    set_bkg_data(0x80, ibm_minimal_dark_TILE_COUNT, ibm_minimal_dark_tiles);

    fill_bkg_rect(0, 0, DEVICE_SCREEN_WIDTH, DEVICE_SCREEN_HEIGHT, 0x80);
    SHOW_BKG;
}

void rendererBegin(Renderer *renderer, uint8_t gridSize)
{
    renderer->size = gridSize;
    fill_bkg_rect(0, 0, DEVICE_SCREEN_WIDTH, DEVICE_SCREEN_HEIGHT, tiles_TILE_COUNT);
    SHOW_BKG;
}

void drawTile(Renderer *renderer, uint8_t x, uint8_t y, uint8_t value)
{
    uint8_t tileStartAdress = 4 * value;
    uint8_t bx = x * 2 + (20 - 2 * renderer->size) / 2; // 20 = SCREENWIDTH in gb tiles
    uint8_t by = y * 2 + (18 - 2 * renderer->size) / 2; // 18 = SCREENHEIGHT in gb tiles
    uint8_t map[] = {tileStartAdress, tileStartAdress + 2, tileStartAdress + 1,
                     tileStartAdress + 3};
    set_bkg_tiles(bx, by, 2, 2, map);
}

void renderer_startAnimation(Renderer *renderer, MoveFrame *frame)
{
    renderer->frame = *frame;

    renderer->animationFrame = 0;
    renderer->animationLength = 8;
    renderer->animating = 1;
}

void renderer_update(Renderer *renderer)
{
    if (!renderer->animating)
        return;

    renderer->animationFrame++;

    if (renderer->animationFrame >= renderer->animationLength)
    {
        renderer->animating = 0;
    }
}

void renderer_draw(Renderer *renderer)
{
    for (int y = 0; y < renderer->size; y++)
    {
        for (int x = 0; x < renderer->size; x++)
        {

            if (renderer->frame.cells[y][x] == 0)
            {
                drawTile(renderer, x, y, 0);
                continue;
            }
            TileMove *tile = &renderer->frame.moves[y][x];

            int drawX = x;
            int drawY = y;

            if (renderer->animating)
            {
                drawX += tile->dx * renderer->animationFrame / renderer->animationLength;
                drawY += tile->dy * renderer->animationFrame / renderer->animationLength;
            }

            drawTile(renderer, drawX, drawY, renderer->frame.cells[y][x]);

            if (drawX != x || drawY != y)
            {
                drawTile(renderer, x, y, 0);
            }
        }
    }
}

void renderer_drawNewCells(Renderer *renderer, MoveFrame *frame)
{
    for (int y = 0; y < renderer->size; y++)
    {
        for (int x = 0; x < renderer->size; x++)
        {
            TileMove *tile = frame->cells[y][x];
            if (tile != 0)
            {
                drawTile(renderer, x, y, tile);
                continue;
            }
        }
    }
}

void renderGameOverAnimation(Renderer *renderer)
{
    uint8_t startTile;
    uint8_t counter;
    uint8_t tileMap0[4] = {0, 2, 1, 3};

    // before we do anything else, lets wait for a second
    counter = 0;
    while (counter < 60)
    {
        counter++;
        vsync();
    }
    counter = 0;

    // first line all empty
    for (int y = 0; y < renderer->size; y++)
    {
        for (int x = 0; x < renderer->size; x++)
        {
            uint8_t bx = x * 2 + ((20 - 2 * renderer->size) / 2); // SCREENWIDTH in gb tiles
            uint8_t by = y * 2 + ((18 - 2 * renderer->size) / 2); // SCREENHEIGHT in gb tiles

            if (y == 1)
            {
                startTile = 0x40;
                uint8_t tileMap[] = {x * 4 + startTile + 0, x * 4 + startTile + 2,
                                     x * 4 + startTile + 1, x * 4 + startTile + 3};
                set_bkg_tiles(bx, by, 2, 2, tileMap);
            }
            else if (y == 2)
            {
                startTile = 0x50;
                uint8_t tileMap[] = {x * 4 + startTile + 0, x * 4 + startTile + 2,
                                     x * 4 + startTile + 1, x * 4 + startTile + 3};
                set_bkg_tiles(bx, by, 2, 2, tileMap);
            }
            else
            {
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

void renderScore(Renderer *renderer, uint16_t score)
{

    char scoreString[16];
    sprintf(scoreString, "Score: %d", score);

    uint8_t index = 0;
    uint8_t tile = 0;
    while (scoreString[index] != '\0')
    {
        char c = scoreString[index];

        tile = getTileAddress(c);

        uint8_t bx = index;

        uint8_t *vramAddress = get_bkg_xy_addr(bx, 17); // always print in the lowest line

        set_vram_byte(vramAddress++, tile);
        index++;
    }
}

void renderMenu(Menu *menu, uint8_t x, uint8_t y)
{
    for (uint8_t i = 0; i < menu->count; i++)
    {
        uint8_t charIndex = 0;
        uint8_t tile = 0;

        uint8_t by = i + y;

        uint8_t *vramAddress = get_bkg_xy_addr(x, by);

        while (menu->options[i][charIndex] != '\0')
        {
            char c = menu->options[i][charIndex];

            if (charIndex == 0 && menu->index == i)
            {
                tile = getTileAddress('>');
                set_vram_byte(vramAddress++, tile);
            }
            else if (charIndex == 0 && menu->index != i)
            {
                tile = getTileAddress(' ');
                set_vram_byte(vramAddress++, tile);
            }
            tile = getTileAddress(c);
            set_vram_byte(vramAddress++, tile);
            charIndex++;
        }
    }
}