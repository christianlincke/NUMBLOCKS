#pragma once

#include <stdint.h>

#define TEXT_WIDTH 20
#define TEXT_HEIGHT 18

typedef struct
{
    char text[TEXT_HEIGHT][TEXT_WIDTH + 1];
    uint8_t cursorX;
    uint8_t cursorY;
} TextBuffer;

void tb_init(TextBuffer *tb);
void tb_putc(TextBuffer *tb, char c);
void tb_newline(TextBuffer *tb);
void tb_dump(TextBuffer *tb);

// void tb_print(TextBuffer *tb, const char *str);
// void tb_scroll(TextBuffer *tb);
// void tb_clear(TextBuffer *tb);
// void tb_draw(TextBuffer *tb);