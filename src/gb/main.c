#include <gb/gb.h>
#include <gbdk/console.h>
#include <gbdk/font.h>
#include <rand.h>


#include "tiles.h"
#include "tiles_empty.h"
#include "start_screen.h"

#include "core/grid.h"
#include "core/textbuffer.h"
#include "render.h"

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

    MoveFrame mFrame;

    Grid grid;
    grid_init(&grid);
    grid_newCell(&grid);
    grid_newCell(&grid);

    renderGrid(&grid);

    uint8_t j;
    uint8_t jMem = 0;
    MoveDirection dir;

    MoveDirection pushActive = 0;
    uint8_t lastPushActive = 0;

    while (1)
    {
        j = joypad();
        j = detectNewJ(&jMem, &j);

        if (j & J_UP)
        {
            dir = MOVE_UP;
            grid_prepare(&grid);
            pushActive = MOVE_UP;
        }
        else if (j & J_DOWN)
        {
            dir = MOVE_DOWN;
            grid_prepare(&grid);
            pushActive = MOVE_DOWN;
        }
        else if (j & J_LEFT)
        {
            dir = MOVE_LEFT;
            grid_prepare(&grid);
            pushActive = MOVE_LEFT;
        }
        else if (j & J_RIGHT)
        {
            dir = MOVE_RIGHT;
            grid_prepare(&grid);
            pushActive = MOVE_RIGHT;
        }

        if (pushActive != MOVE_NONE)
        {
            pushActive = grid_move(&grid, &mFrame, dir);
        }

        if (pushActive == MOVE_NONE && lastPushActive != MOVE_NONE)
        {
            grid_newCell(&grid);
        }

        lastPushActive = pushActive;

        renderGrid(&grid);
        vsync();
    }
}
