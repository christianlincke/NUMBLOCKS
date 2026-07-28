#pragma once

#include "textbuffer.h"

#include <stdint.h>

#define GRID_SIZE 4

// TODO keep track if a puch in a direction actually did, new cells should only be generated if the grid has changed

typedef enum {
    GRID_UP,
    GRID_DOWN,
    GRID_LEFT,
    GRID_RIGHT
} PushDirection;

typedef struct {
    uint8_t cells[GRID_SIZE][GRID_SIZE];
    uint8_t hasMerged[GRID_SIZE][GRID_SIZE];
} Grid;

void grid_init(Grid *grid);
void grid_push(Grid *grid, PushDirection dir);
void grid_newCell(Grid *grid);
void grid_dump(Grid *grid, TextBuffer *buffer);