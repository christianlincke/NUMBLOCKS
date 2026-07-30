#include "game.h"

#include <gb/gb.h>
#include <rand.h>

#include "core/grid.h"
#include "render.h"
#include "joypad.h"
#include "render.h"
#include "menu.h"

void startScreen()
{
    renderStartScreen();

    while (!(joypad() & J_START))
    {
        vsync();
    }
}

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

uint16_t runGame(uint8_t gridSize)
{
    // init Grid, MoveFrame and Renderer
    Grid grid;
    grid_init(&grid, gridSize);

    MoveFrame moveFrame;
    moveFrame_init(&moveFrame, gridSize);

    Renderer renderer;
    rendererBegin(&renderer, gridSize);

    // init random
    initrand(sys_time);

    // spawn two cells
    moveFrame = grid_newCell(&grid);
    renderGrid(&renderer, &moveFrame);
    moveFrame = grid_newCell(&grid);
    renderGrid(&renderer, &moveFrame);

    uint8_t j = 0;
    uint8_t jMem = 0;
    MoveDirection dir;

    MoveDirection startMove = MOVE_NONE;
    MoveDirection lastPushActive = MOVE_NONE;

    while (1)
    {
        j = joypadDebounce(&jMem);

        if (j & J_UP)
        {
            dir = MOVE_UP;
            grid_prepare(&grid);
            startMove = MOVE_UP;
        }
        else if (j & J_DOWN)
        {
            dir = MOVE_DOWN;
            grid_prepare(&grid);
            startMove = MOVE_DOWN;
        }
        else if (j & J_LEFT)
        {
            dir = MOVE_LEFT;
            grid_prepare(&grid);
            startMove = MOVE_LEFT;
        }
        else if (j & J_RIGHT)
        {
            dir = MOVE_RIGHT;
            grid_prepare(&grid);
            startMove = MOVE_RIGHT;
        }
        else if (j & J_START)
        {
            uint8_t cont = gameMenu();
            if (!cont) {
                return grid_sumCells(&grid);
            }
        }
        else
        {
            startMove = MOVE_NONE;
        }

        // new joypad input!
        if (moveFrame.moveActive != MOVE_NONE || startMove != MOVE_NONE)
        {
            moveFrame = grid_move(&grid, dir);
        }

        // move is done, make a new cell
        if (moveFrame.moveActive == MOVE_NONE && lastPushActive != MOVE_NONE)
        {
            moveFrame = grid_newCell(&grid);
        }

        lastPushActive = moveFrame.moveActive;

        // render
        renderGrid(&renderer, &moveFrame);
        renderScore(&renderer, grid_sumCells(&grid));

        if (grid_checkGameOver(&grid))
        {
            renderGameOverAnimation(&renderer);
            while (!(joypad() & J_START))
            {
                vsync();
            }
            return grid_sumCells(&grid);
        }
        vsync();
    }
}