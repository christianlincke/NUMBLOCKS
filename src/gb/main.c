// GBDK
#include <gb/gb.h>
#include <gbdk/console.h>
#include <gbdk/font.h>
#include <rand.h>

// LOGIC
#include "core/grid.h"
#include "render.h"

// ASSETS
#include "tiles.h"
#include "tiles_empty.h"
#include "start_screen.h"

void showStartScreen(){
    // Load Background tiles and then map
    set_bkg_data(0, start_screen_TILE_COUNT, start_screen_tiles);
    set_bkg_tiles(0, 0, 20, 18, start_screen_map);

	// Turn the background map on to make it visible
    SHOW_BKG;

    while (!(joypad() & J_START)) {
        vsync();
    }
}

uint8_t detectNewJ(uint8_t *jMem, uint8_t *j)
{
    if (*j == *jMem)
    {
        return 0;
    }
    *jMem = *j;
    return *j;
}

void main(void)
{
    showStartScreen();
    initrand(sys_time);

    SHOW_BKG;
    set_bkg_data(0, tiles_TILE_COUNT, tiles_tiles);
    set_bkg_data(tiles_TILE_COUNT, tiles_empty_TILE_COUNT, tiles_empty_tiles);
    fill_bkg_rect(0, 0, DEVICE_SCREEN_WIDTH, DEVICE_SCREEN_HEIGHT, tiles_TILE_COUNT);

    // init Grid, MoveFrame and Renderer
    Grid grid;
    grid_init(&grid);

    MoveFrame moveFrame;
    moveFrame_clear(&moveFrame);

    Renderer renderer;
    rendererBegin(&renderer);

    // spawn two cells
    moveFrame = grid_newCell(&grid);
    renderGrid(&renderer, &moveFrame);

    moveFrame = grid_newCell(&grid);
    renderGrid(&renderer, &moveFrame);

    uint8_t j;
    uint8_t jMem = 0;
    MoveDirection dir;

    MoveDirection startMove = MOVE_NONE;
    MoveDirection lastPushActive = MOVE_NONE;

    while (1)
    {
        j = joypad();
        j = detectNewJ(&jMem, &j);

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
        else {
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

        renderGrid(&renderer, &moveFrame);
        vsync();
    }
}
