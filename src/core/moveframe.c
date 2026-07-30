#include "moveframe.h"


void moveFrame_init(MoveFrame *frame, uint8_t gridSize) {
    frame->size = gridSize;
    moveFrame_clear(frame);
}

void moveFrame_clear(MoveFrame *frame) {
    frame->moveActive = MOVE_NONE;
    for (uint8_t y = 0; y < frame->size; y++) {
        for (uint8_t x = 0; x < frame->size; x++) {
            frame->cells[y][x] = -1;
        }
    }
}