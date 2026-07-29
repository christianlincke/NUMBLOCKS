#include <gb/gb.h>
#include <gbdk/console.h>
#include <gbdk/font.h>
#include <rand.h>
#include "Font.h"
#include "tiles.h"

#include "core/grid.h"
#include "core/textbuffer.h"
#include "render.h"

uint8_t GetCharacterVRamTile(char character)
{
    uint8_t vramTile = character - 32;
    return vramTile;
}

void DrawText(TextBuffer *buffer)
{
    for (int y = 0; y < TEXT_HEIGHT; y++)
    {
        uint8_t *vramAddress = get_bkg_xy_addr(0, y);

        for (int x = 0; x < TEXT_WIDTH; x++)
        {
            char character = buffer->text[y][x];
            uint8_t vramTile = GetCharacterVRamTile(character);
            set_vram_byte(vramAddress++, vramTile);
        }
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
    initrand(sys_time);

    SHOW_BKG;
    // font_init();
    // font_load(font_ibm);
    set_bkg_data(0, tiles_TILE_COUNT, tiles_tiles);
    fill_bkg_rect(0, 0, DEVICE_SCREEN_WIDTH, DEVICE_SCREEN_HEIGHT, 0);

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
