#include "game.h"

// #ifdef TARGET_GB
#include <gb/gb.h>
#include <gbdk/platform.h>
#include <rand.h>
// #endif

#ifdef TARGET_CLI
#include <unistd.h>
#endif

#include "core/display.h"
#include "core/grid.h"
#include "core/joypad.h"
#include "core/menu.h"
#include "core/render.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void startScreen()
{
    showStartScreen();
    uint8_t jMem = 0;

    while (!(joypadDebounce(&jMem) & J_START))
    {
#ifdef TARGET_GB
        vsync();
#endif
    }
}

void showScores(Scores scores)
{
    waitpadup();
    clearScreen();
    const char *categories[] = {"4x4", "5x5", "6x6", "7x7", "8x8"};

    for (uint8_t i = 0; i < 5; i++)
    {
        char scoreString[12];
        sprintf(scoreString, "%s: %d", categories[i], scores.scores[i]);
        scoreString[sizeof(scoreString) - 1] = '\0';
        printString(6, 7 + i, scoreString);
    }
    uint8_t jMem = 0;
    uint8_t j = 0;
    while (1)
    {
        j = joypadDebounce(&jMem);
        if (j & J_START || j || j & J_B)
        {
            return;
        }
#ifdef TARGET_GB
        vsync();
#endif
    }
}

uint16_t runGame(const uint8_t gridSize)
{
    // init grid, MoveFrame and Renderer
    MoveFrame moveFrame;
    moveFrame_init(&moveFrame, gridSize);

    Grid grid;
    grid_init(&grid, &moveFrame, gridSize);

    Renderer renderer;
    renderer_init(&renderer, &grid, &moveFrame);
    renderer_takeSnapshot(&renderer);
    renderer_drawAll(&renderer);

    // init random
#ifdef TARGET_GB
    initrand(sys_time);
#endif

    // spawn two cells
    grid_newCell(&grid, 2);
    renderer_drawDiff(&renderer);

    uint8_t j = 0;
    uint8_t jMem = 0;
    uint8_t moveActive = 0;
    uint8_t prevMove = 0;
    uint8_t newCell = 0;
    uint8_t gameOver = 0;
    int timeStamp = 0;
    static const uint8_t numNewCells[5] = {1,1,1,2,2};
    renderScore(grid_calcScore(&grid), gridSize);

    while (1)
    {
        // get input
        j = joypadDebounce(&jMem);

        prevMove = moveActive;

        // set currMove and prepeare grid
        if (j & J_UP && !renderer.animating && !moveActive && !gameOver)
        {
            moveActive = MOVE_UP;
            grid_prepare(&grid, MOVE_UP);
        }
        else if (j & J_DOWN && !renderer.animating && !moveActive && !gameOver)
        {
            moveActive = MOVE_DOWN;
            grid_prepare(&grid, MOVE_DOWN);
        }
        else if (j & J_LEFT && !renderer.animating && !moveActive && !gameOver)
        {
            moveActive = MOVE_LEFT;
            grid_prepare(&grid, MOVE_LEFT);
        }
        else if (j & J_RIGHT && !renderer.animating && !moveActive && !gameOver)
        {
            moveActive = MOVE_RIGHT;
            grid_prepare(&grid, MOVE_RIGHT);
        }
        else if (j & J_START && !renderer.animating && !moveActive && !gameOver)
        {
            uint8_t cont = gameMenu();
            if (!cont)
            {
                return grid_calcScore(&grid);
            }
            renderer_drawAll(&renderer);
        }
        else if (j & J_START && !renderer.animating && gameOver)
        {
            return grid_calcScore(&grid);
        }

        // move the cell
        if (moveActive && !renderer.animating && !gameOver)
        {

            // timeStamp = sys_time;
            renderer_takeSnapshot(&renderer);
            moveActive = grid_move(&grid);
            // timeStamp = sys_time - timeStamp;
            if (moveActive)
            {
                renderer_startAnimation(&renderer, ANIMATION_MOVE);
            }
        }
        else if (!renderer.animating && !gameOver)
        {
            prevMove = moveActive;
        }

        if (prevMove && !moveActive && !renderer.animating && !gameOver)
        {
            renderer_takeSnapshot(&renderer);
            moveFrame_clear(&moveFrame);
            grid_newCell(&grid, numNewCells[gridSize - 4]);
            moveActive = 0;
            newCell = 1;
            renderer_startAnimation(&renderer, ANIMATION_NEWCELL);
        }

        // check game over
        if (!moveActive && !gameOver && grid_checkGameOver(&grid) && !renderer.animating)
        {
            gameOver = 1;
            renderer_takeSnapshot(&renderer);
            renderer_startAnimation(&renderer, ANIMATION_GAMEOVER);
        }

        renderer_update(&renderer); // r.a = 0 if animation done
        renderer_drawDiff(&renderer);

        // TODO only calc score once when move is finished and we have a new tile
        // takes up much more processing power than i expected
        // also, the sprintf() inside renderScore call uses CPU, maybe replace?
        if (newCell && !renderer.animating)
        {
            renderScore(grid_calcScore(&grid), gridSize);
            newCell = 0;
        }

        // for debug / optim
        // renderTimestamp(timeStamp);

#ifdef TARGET_GB
        vsync();
#endif

#ifdef TARGET_CLI
        usleep(50000);
#endif
    }
}
