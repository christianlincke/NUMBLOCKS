#include "game.h"

#ifdef TARGET_GB
#include <gb/gb.h>
#include <rand.h>
#endif

#ifdef TARGET_CLI
#include <unistd.h>
#endif

#include "core/grid.h"
#include "core/joypad.h"
#include "core/menu.h"
#include "core/render.h"
#include "core/display.h"

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
    Grid grid;
    grid_init(&grid, gridSize);

    MoveFrame moveFrame;
    moveFrame_init(&moveFrame, gridSize);

    Renderer renderer;
    renderer_init(&renderer, gridSize, &moveFrame);

    // init random
#ifdef TARGET_GB
    initrand(sys_time);
#endif

    // spawn two cells
    grid_newCell(&grid, &moveFrame, 2);
    renderer_drawNewCells(&renderer, &moveFrame);

    uint8_t j = 0;
    uint8_t jMem = 0;

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

        if (moveActive && !renderer.animating)
        {
            moveFrame_clear(&moveFrame);
            grid_newCell(&grid, &moveFrame, 1);
            moveActive = 0;
            renderer_drawNewCells(&renderer, &moveFrame);
        }


        // if (grid_checkGameOver(&grid))
        // {
        //     renderGameOverAnimation(&renderer);
        //     while (!(joypad() & J_START))
        //     {

        //         vsync();
        //     }
        //     return grid_sumCells(&grid);
        // }


        renderer_update(&renderer);
        renderer_draw(&renderer);
        // renderScore(grid_sumCells(&grid));

#ifdef TARGET_GB
        vsync();
#endif

#ifdef TARGET_CLI
        usleep(50000);
#endif
    }
}
