#pragma once

// just a couple of defines
#define GRID_SIZE 4 // size of the playing field

// move directions
typedef enum {
    MOVE_NONE = 0,
    MOVE_UP = 1,
    MOVE_DOWN = 2,
    MOVE_LEFT = 3,
    MOVE_RIGHT = 4
} MoveDirection;