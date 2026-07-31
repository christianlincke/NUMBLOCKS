#pragma once
/**
 * @file moveframe.h
 * @author christian
 * @brief data structure to tell the animator / renderer that to do
 * @version 0.1
 * @date 2026-07-29
 *
 * @copyright Copyright (c) 2026
 *
 */


#include <stdint.h>

// move directions
typedef enum {
    MOVE_NONE = 0,
    MOVE_UP = 1,
    MOVE_DOWN = 2,
    MOVE_LEFT = 3,
    MOVE_RIGHT = 4
} MoveDirection;

typedef struct {
    uint8_t value;

    uint8_t fromX;
    uint8_t fromY;

    uint8_t toX;
    uint8_t toY;

    uint8_t merge;
} TileMove;

typedef struct
{
    int8_t cells[8][8];
    MoveDirection moveActive;
    TileMove moves[8][8];
    uint8_t size;
} MoveFrame;

/**
 * @brief clear a TileMove object
 * 
 */
void tileMove_clear(TileMove *move);

/**
 * @brief initialize move frame object
 * 
 * @param frame 
 * @param gridSize 
 */
void moveFrame_init(MoveFrame *frame, uint8_t gridSize);

/**
 * @brief initialize / clear the move event
 *
 * @param frame
 */
void moveFrame_clear(MoveFrame *frame);