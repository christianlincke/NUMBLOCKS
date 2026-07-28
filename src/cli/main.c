#include <stdio.h>
#include "core/textbuffer.h"
#include "core/grid.h"
#include "kbdinput.h"
#include "render.h"

int main(int argc, char *argv[])
{
    TextBuffer buffer;
    tb_init(&buffer);

    Grid grid;
    grid_init(&grid);
    grid_newCell(&grid);
    grid_newCell(&grid);
    grid_dump(&grid, &buffer);
    tb_dump(&buffer);

    while (1) {
        int ch = getch();
        switch (ch)
        {
        case 'w':
            grid_push(&grid, GRID_UP);
            grid_newCell(&grid);
            break;
        case 'a':
            grid_push(&grid, GRID_LEFT);
            grid_newCell(&grid);
            break;
        case 's':
            grid_push(&grid, GRID_DOWN);
            grid_newCell(&grid);
            break;
        case 'd':
            grid_push(&grid, GRID_RIGHT);
            grid_newCell(&grid);
            break;
        case 'q':
            return 0;
            break;
        default:
            break;
        }
        
        grid_dump(&grid, &buffer);
        tb_dump(&buffer);
    }

    return 0;
}