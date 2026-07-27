#pragma once

#include <stdint.h>

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

typedef struct {
    uint16_t cells[4][4];
    uint16_t hasMerged[4][4];
} Grid;

void grid_init(Grid *grid);
void grid_push(Grid *grid, Direction dir);
void grid_newCell(Grid *grid);