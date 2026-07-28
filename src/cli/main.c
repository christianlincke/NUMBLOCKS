#include <stdio.h>
#include "core/textbuffer.h"
#include "core/grid.h"
#include "kbdinput.h"
#include "render.h"

int main(int argc, char *argv[])
{
    TextBuffer buffer;
    tb_init(&buffer);

    MoveFrame mf;

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
            while(grid_move(&grid, &mf, MOVE_UP)){};
            grid_newCell(&grid);
            break;
        case 'a':
            while(grid_move(&grid, &mf, MOVE_LEFT)){};
            grid_newCell(&grid);
            break;
        case 's':
            while(grid_move(&grid, &mf, MOVE_DOWN)){};
            grid_newCell(&grid);
            break;
        case 'd':
            while(grid_move(&grid, &mf, MOVE_RIGHT)){};
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