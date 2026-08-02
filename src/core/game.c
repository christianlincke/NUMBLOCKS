#include "game.h"

#ifdef TARGET_GB
#include <gb/gb.h>
#include <rand.h>
#endif

#ifdef TARGET_CLI
#include <unistd.h>
#endif

#include "core/display.h"
#include "core/grid.h"
#include "core/joypad.h"
#include "core/menu.h"
#include "core/render.h"

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

uint16_t runGame(uint8_t gridSize)
{
    // init grid, MoveFrame and Renderer
    MoveFrame moveFrame;
    moveFrame_init(&moveFrame, gridSize);

    Grid grid;
    grid_init(&grid, &moveFrame, gridSize);

    Renderer renderer;
    renderer_init(&renderer, &grid, &moveFrame);

    // init random
#ifdef TARGET_GB
    initrand(sys_time);
#endif

    // spawn two cells
    grid_newCell(&grid, 2);
    renderer_draw(&renderer);

    uint8_t j = 0;
    uint8_t jMem = 0;

    uint8_t moveActive = 0;
    uint8_t prevMove = 0;

    uint8_t gameOver = 0;

    while (1)
    {
        // get input
        j = joypadDebounce(&jMem);

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
        else if (j & J_START && !renderer.animating)
        {
            uint8_t cont = gameMenu();
            if (!cont)
            {
                return grid_sumCells(&grid);
            }
        }
        else if (j & J_START && !renderer.animating && gameOver)
        {
            return grid_sumCells(&grid);
        }

        // move the cell
        if (moveActive && !renderer.animating)
        {
            moveActive = grid_move(&grid);
        }

        if (prevMove && !moveActive && !renderer.animating)
        {
            moveFrame_clear(&moveFrame);
            grid_newCell(&grid, 1);
            moveActive = 0;
        }

        if (!moveActive && !gameOver && !renderer.animating)
        {
            gameOver = grid_checkGameOver(&grid);
        }

        prevMove = moveActive;


        // renderer_update(&renderer);
        renderer_draw(&renderer);
        renderScore(grid_sumCells(&grid));

#ifdef TARGET_GB
        vsync();
#endif

#ifdef TARGET_CLI
        usleep(50000);
#endif
    }
}
