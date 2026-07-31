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
    grid_newCell(&grid, &moveFrame, 1);
    grid_newCell(&grid, &moveFrame, 1);
    renderer_startAnimation(&renderer, &moveFrame);
    renderer_draw(&renderer);

    uint8_t j = 0;
    uint8_t jMem = 0;
    MoveDirection dir;

    MoveDirection startMove = MOVE_NONE;

    uint8_t moveActive = 0; // wait for animation to finish before drawing new cell

    while (1)
    {
        j = joypadDebounce(&jMem);

        if (j & J_UP && !renderer.animating)
        {
            grid_prepare(&grid);
            grid_move(&grid, &moveFrame, MOVE_UP);
            grid_newCell(&grid, &moveFrame, 1);
            renderer_startAnimation(&renderer, &moveFrame);
            moveActive = 1;
        }
        else if (j & J_DOWN && !renderer.animating)
        {
            grid_prepare(&grid);
            grid_move(&grid, &moveFrame, MOVE_DOWN);
            grid_newCell(&grid, &moveFrame, 1);
            renderer_startAnimation(&renderer, &moveFrame);
            moveActive = 1;
        }
        else if (j & J_LEFT && !renderer.animating)
        {
            grid_prepare(&grid);
            grid_move(&grid, &moveFrame, MOVE_LEFT);
            grid_newCell(&grid, &moveFrame, 1);
            renderer_startAnimation(&renderer, &moveFrame);
            moveActive = 1;
        }
        else if (j & J_RIGHT && !renderer.animating)
        {
            grid_prepare(&grid);
            grid_move(&grid, &moveFrame, MOVE_RIGHT);
            grid_newCell(&grid, &moveFrame, 1);
            renderer_startAnimation(&renderer, &moveFrame);
            moveActive = 1;
        }
        else if (j & J_START && !renderer.animating)
        {
            uint8_t cont = gameMenu();
            if (!cont)
            {
                return grid_sumCells(&grid);
            }
        }

        if (moveActive && !renderer.animating) {
            moveFrame_clear(&moveFrame);
            grid_newCell(&grid, &moveFrame, 1);
            moveActive = 0;
            renderer_drawNewCells(&renderer, &moveFrame);
        }

        if (grid_checkGameOver(&grid))
        {
            renderGameOverAnimation(&renderer);
            while (!(joypad() & J_START))
            {
                vsync();
            }
            return grid_sumCells(&grid);
        }


        renderer_update(&renderer);
        renderer_draw(&renderer);
        renderScore(&renderer, grid_sumCells(&grid));
        vsync();
    }
}