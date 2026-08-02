#include "moveframe.h"

void tileMove_clear(TileMove *move) {
    move->dx = 0;
    move->dy = 0;
    move->merge = 0;
}

void moveFrame_init(MoveFrame *frame, uint8_t gridSize) {
    frame->size = gridSize;
    moveFrame_clear(frame);
}

void moveFrame_clear(MoveFrame *frame) {
    frame->moveActive = MOVE_NONE;
    for (uint8_t y = 0; y < frame->size; y++) {
        for (uint8_t x = 0; x < frame->size; x++) {
            tileMove_clear(&frame->moves[y][x]);
        }
    }
}