#pragma once

#include "textbuffer.h"

#include <stdint.h>

typedef enum {
    GRID_UP,
    GRID_DOWN,
    GRID_LEFT,
    GRID_RIGHT
} Direction;

typedef struct {
    uint8_t cells[4][4];
    uint8_t hasMerged[4][4];
} Grid;

void grid_init(Grid *grid);
void grid_push(Grid *grid, Direction dir);
void grid_newCell(Grid *grid);
void grid_dump(Grid *grid, TextBuffer *buffer);