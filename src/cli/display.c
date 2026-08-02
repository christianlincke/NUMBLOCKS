#include "core/display.h"

#include <stdio.h>

void loadAssets()
{
    return;
}

uint8_t getTileAddress(const char c)
{
    return 0;
}

void drawTile(uint8_t gridSize, uint8_t x, uint8_t y, uint8_t value)
{
    const char characters[] = "0123456789ABCDEF";
    char c = characters[value];
    // Move cursor to row y, column x
    printf("\033[%d;%dH%c", y, x, value);
    fflush(stdout);
}

void showStartScreen()
{
    printf("NUMBLOCKS\n");
    printf("PRESS [.]\n");

    return;
}

void clearScreen(void)
{
    printf("\033[2J");  // Clear the screen
    printf("\033[H");   // Move cursor to top-left
    fflush(stdout);
}

void showBackground() {
    return;
}

void printString(uint8_t x, uint8_t y, const char *text)
{
    uint8_t i = 0;
    while(text[i] != '\0') {
        drawTile(4, 5, i, text[i]);
    }
}