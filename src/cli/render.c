#include "render.h"

#include <stdio.h>
#include <math.h>

const char characters[] = " 123456789ABCDEF";

void clearScreen(void)
{
    printf("\033[2J");  // Clear the screen
    printf("\033[H");   // Move cursor to top-left
    fflush(stdout);
}

void render(Grid *grid) {
    clearScreen();
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            uint8_t index = grid->cells[y][x];
            printf("%c", characters[index]);
        }
        printf("\n");
    }
}