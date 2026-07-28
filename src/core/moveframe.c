#include "moveframe.h"


void moveFrame_clear(MoveFrame *frame) {
    for (uint8_t y = 0; y < GRID_SIZE; y++) {
        for (uint8_t x = 0; x < GRID_SIZE; x++) {
            frame->dx[y][x] = 0;
            frame->dy[y][x] = 0;
            frame->merged[y][x] = 0;
            frame->value[y][x] = 0;
        }
    }
}