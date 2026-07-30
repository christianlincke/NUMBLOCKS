#include "game.h"

#include <gb/gb.h>
#include <rand.h>

#include "core/grid.h"
#include "render.h"
#include "joypad.h"
#include "render.h"

void startScreen()
{
    renderStartScreen();

    while (!(joypad() & J_START))
    {
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
        else
        {
            startMove = MOVE_NONE;
        }

        if (moveFrame.moveActive != MOVE_NONE || startMove != MOVE_NONE)
        {
            moveFrame = grid_move(&grid, dir);
        }

        if (moveFrame.moveActive == MOVE_NONE && lastPushActive != MOVE_NONE)
        {
            moveFrame = grid_newCell(&grid);
        }

        lastPushActive = moveFrame.moveActive;

        if (grid_checkGameOver(&grid))
        {
            renderGameOverAnimation(&renderer);
            while (!(joypad() & J_START))
            {
                vsync();
            }
            return grid_sumCells(&grid);
        }

        renderGrid(&renderer, &moveFrame);
        vsync();
    }
}