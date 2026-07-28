#include "textbuffer.h"

#include <stdio.h>


void tb_init(TextBuffer *tb)
{
    for (int y = 0; y < TEXT_HEIGHT; y++)
    {
        for (int x = 0; x < TEXT_WIDTH + 1; x++)
            tb->text[y][x] = ' ';

        tb->text[y][20] = '\0';
    }

    tb->cursorX = 0;
    tb->cursorY = 0;
}

void tb_putc(TextBuffer *tb, char c) {
    tb->text[tb->cursorY][tb->cursorX] = c;
    
    tb->cursorX++;
    if (tb->cursorX >= TEXT_WIDTH) {
        tb->cursorX = 0;
        tb->cursorY++;
        if (tb->cursorY >= TEXT_HEIGHT) {
            tb->cursorY = 0; // Wrap around to the top
        }
    }
}

void tb_newline(TextBuffer *tb) {
    tb->cursorY++;
    if (tb->cursorY >= TEXT_HEIGHT) {
        tb->cursorY = 0;
    }
    tb->cursorX = 0;
}

void tb_dump(TextBuffer *tb) {
    for (uint8_t y = 0; y < TEXT_HEIGHT; y++)
    {
        printf("|%s|\n", tb->text[y]);
    }
}