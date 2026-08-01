#include "core/display.h"

#include <gb/gb.h>

// ASSETS
#include "ibm_minimal_dark.h"
#include "start_screen.h"
#include "tiles.h"
#include "tiles_empty.h"

#include <stdio.h>


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

void drawTile(uint8_t gridSize, uint8_t x, uint8_t y, uint8_t value)
{
    uint8_t tileStartAdress = 4 * value;
    uint8_t bx = x * 2 + (20 - 2 * gridSize) / 2; // 20 = SCREENWIDTH in gb tiles
    uint8_t by = y * 2 + (18 - 2 * gridSize) / 2; // 18 = SCREENHEIGHT in gb tiles
    uint8_t map[] = {tileStartAdress, tileStartAdress + 2, tileStartAdress + 1,
                     tileStartAdress + 3};
    set_bkg_tiles(bx, by, 2, 2, map);
}

void showStartScreen()
{
    // Load Background tiles and then map
    set_bkg_data(0, start_screen_TILE_COUNT, start_screen_tiles);
    set_bkg_tiles(0, 0, 20, 18, start_screen_map);
}

void clearScreen() {
    fill_bkg_rect(0, 0, DEVICE_SCREEN_WIDTH, DEVICE_SCREEN_HEIGHT, 0x80);
}

void showBackground() {
    SHOW_BKG;
}

void renderScore(uint16_t score)
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

void printString(uint8_t x, uint8_t y, const char *text)
{
    uint8_t tile = 0;
    uint8_t charIndex = 0;
    while (text[charIndex] != '\0')
        {
            uint8_t *vramAddress = get_bkg_xy_addr(x, y);
            char c = text[charIndex];
            tile = getTileAddress(c);
            set_vram_byte(vramAddress++, tile);
            charIndex++;
        }
}