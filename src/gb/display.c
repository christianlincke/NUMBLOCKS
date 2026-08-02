#include "core/display.h"

#include <gb/gb.h>

// ASSETS
#include "ibm_minimal_dark.h"
#include "start_screen.h"
#include "tiles.h"
#include "tiles_empty.h"

#include <stdio.h>

static const uint8_t gridOffsetX[] = {[4] = 6, [5] = 5, [6] = 4, [7] = 3, [8] = 2}; // (20 - 2 * gridSize) / 2
static const uint8_t gridOffsetY[] = {[4] = 5, [5] = 4, [6] = 3, [7] = 2, [8] = 1}; // (18 - 2 * gridSize) / 2

void loadAssets()
{
    // load tiles, empties and font
    set_bkg_data(0x00, tiles_TILE_COUNT, tiles_tiles); // 0x80 - 0x5F
    set_bkg_data(0x60, tiles_empty_TILE_COUNT, tiles_empty_tiles);
    set_bkg_data(0x80, ibm_minimal_dark_TILE_COUNT, ibm_minimal_dark_tiles);

    fill_bkg_rect(0, 0, DEVICE_SCREEN_WIDTH, DEVICE_SCREEN_HEIGHT, 0x80);
    SHOW_BKG;
}

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

void drawTile(const uint8_t gridSize, const uint8_t x, const uint8_t y, const uint8_t value)
{
    uint8_t tileStartAdress = 4 * value;
    uint8_t bx = x * 2 + gridOffsetX[gridSize];
    uint8_t by = y * 2 + gridOffsetY[gridSize]; 
    uint8_t map[] = {tileStartAdress, tileStartAdress + 2, tileStartAdress + 1,
                     tileStartAdress + 3};
    set_bkg_tiles(bx, by, 2, 2, map);
}

void showStartScreen()
{
    // Load Background tiles and then map
    set_bkg_data(0, start_screen_TILE_COUNT, start_screen_tiles);
    set_bkg_tiles(0, 0, 20, 18, start_screen_map);
    SHOW_BKG;
}

void clearScreen()
{
    fill_bkg_rect(0, 0, DEVICE_SCREEN_WIDTH, DEVICE_SCREEN_HEIGHT, 0x80);
}

void showBackground()
{
    SHOW_BKG;
}

void printString(uint8_t x, uint8_t y, const char *text)
{
    uint8_t tile = 0;
    uint8_t charIndex = 0;
    uint8_t *vramAddress = get_bkg_xy_addr(x, y);
    while (text[charIndex] != '\0')
    {
        char c = text[charIndex];
        tile = getTileAddress(c);
        set_vram_byte(vramAddress++, tile);
        charIndex++;
    }
}