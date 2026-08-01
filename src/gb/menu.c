#include "core/menu.h"

#include "core/render.h"
#include "core/joypad.h"

#include <gb/gb.h>

uint8_t mainMenu()
{
    waitpadup();

    const char *mainOptions[] = {"4x4", "5x5", "6x6", "7x7", "8x8"};

    uint8_t mainValues[] = {4, 5, 6, 7, 8};

    Menu menu = {
        .options = mainOptions,
        .values = mainValues,
        .count = 5,
        .index = 0};

    uint8_t j = 0;
    uint8_t jMem = 0;

    while (1)
    {
        j = joypadDebounce(&jMem);

        if (j & J_UP && menu.index > 0)
        {
            menu.index += -1;
        }
        else if (j & J_DOWN && menu.index < menu.count - 1)
        {
            menu.index += 1;
        }

        if (j & J_A)
        {
            return menu.values[menu.index];
        }

        renderMenu(&menu, 7, 6);
        vsync();
    }
}

uint8_t gameMenu()
{
    waitpadup();
    const char *gameOptions[] = {"continue", "quit"};

    uint8_t gameValues[] = {1, 0};

    Menu menu = {
        .options = gameOptions,
        .values = gameValues,
        .count = 2,
        .index = 0};

    uint8_t j = 0;
    uint8_t jMem = 0;

    fill_bkg_rect(0, 0, DEVICE_SCREEN_WIDTH, DEVICE_SCREEN_HEIGHT, 0x80);

    while (1)
    {
        j = joypadDebounce(&jMem);

        if (j & J_UP && menu.index > 0)
        {
            menu.index += -1;
        }
        else if (j & J_DOWN && menu.index < menu.count - 1)
        {
            menu.index += 1;
        }

        if (j & J_A)
        {
            fill_bkg_rect(0, 0, DEVICE_SCREEN_WIDTH, DEVICE_SCREEN_HEIGHT, 0x80);
            return menu.values[menu.index];
        }

        renderMenu(&menu, 4, 6);
        vsync();
    }
}