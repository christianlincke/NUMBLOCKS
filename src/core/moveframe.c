#include "moveframe.h"


void moveFrame_clear(MoveFrame *frame) {
    for (uint8_t y = 0; y < GRID_SIZE; y++) {
        for (uint8_t x = 0; x < GRID_SIZE; x++) {
            frame->cells[y][x] = 0;
            frame->moveActive = MOVE_NONE; 
        }
    }
}