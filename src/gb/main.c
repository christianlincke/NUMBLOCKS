// GBDK
#include <gb/gb.h>
#include <gbdk/console.h>
#include <gbdk/font.h>
#include <rand.h>

// game stuff
#include "core/grid.h"
#include "render.h"

#define GRID_SIZE 4


void startScreen()
{
    renderStartScreen();

    while (!(joypad() & J_START))
    {
        vsync();
    }
}

uint8_t joypadDebounce(uint8_t *jMem)
{
    uint8_t j = joypad();
    if (j == *jMem)
    {
        return 0;
    }
    *jMem = j;
    return j;
}

void main(void)
{
    startScreen();
    initrand(sys_time);

    // init Grid, MoveFrame and Renderer
    Grid grid;
    grid_init(&grid, GRID_SIZE);

    MoveFrame moveFrame;
    moveFrame_init(&moveFrame, GRID_SIZE);

    Renderer renderer;
    rendererBegin(&renderer, GRID_SIZE);

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

        if (j & J_START)
        {
            renderGameOverAnimation();
            while (!(joypad() & J_START))
            {
                vsync();
            }
        }

        renderGrid(&renderer, &moveFrame);
        vsync();
    }
}
