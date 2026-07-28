#include <gb/gb.h>
#include <gbdk/console.h>
#include <gbdk/font.h>
#include <rand.h>
#include "Font.h"

#include "core/grid.h"
#include "core/textbuffer.h"

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
    font_init();
    font_load(font_ibm);
    fill_bkg_rect(0, 0, DEVICE_SCREEN_WIDTH, DEVICE_SCREEN_HEIGHT, 0);

    TextBuffer buffer;
    tb_init(&buffer);

    Grid grid;
    grid_init(&grid);
    grid_newCell(&grid);
    grid_newCell(&grid);

    grid_dump(&grid, &buffer);

    DrawText(&buffer);

    uint8_t j;
    uint8_t jMem = 0;
    Direction dir;

    while (1)
    {
        j = joypad();
        j = detectNewJ(&jMem, &j);

        if (j & J_UP)
        {
            dir = GRID_UP;
        }
        else if (j & J_DOWN)
        {
            dir = GRID_DOWN;
        }
        else if (j & J_LEFT)
        {
            dir = GRID_LEFT;
        }
        else if (j & J_RIGHT)
        {
            dir = GRID_RIGHT;
        }

        if (j)
        {
            grid_push(&grid, dir);
            grid_newCell(&grid);
            grid_dump(&grid, &buffer);
        }

        DrawText(&buffer);
        vsync();
    }
}
