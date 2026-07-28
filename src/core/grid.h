#pragma once

#include "textbuffer.h"

#include <stdint.h>

// TODO keep track if a puch in a direction actually did, new cells should only be generated if the grid has changed

typedef enum {
    GRID_UP,
    GRID_DOWN,
    GRID_LEFT,
    GRID_RIGHT
} PushDirection;

typedef struct {
    uint8_t cells[4][4];
    uint8_t hasMerged[4][4];
} Grid;

void grid_init(Grid *grid);
void grid_push(Grid *grid, PushDirection dir);
void grid_newCell(Grid *grid);
void grid_dump(Grid *grid, TextBuffer *buffer);