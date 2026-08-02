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

/**
 * @brief encoded data for tile, containing dx, dy and merge
 * 0bMMMMYYXX
 * MMMM is MERGE
 * 0000 for no merge
 * 0011 (hex 3) for merge into
 * 1100 (hex C) for tile that moves and then merges into other tile
 * YY is dY, XX is dX
 * 00 for 0
 * 10 for -1
 * 01 for 1
 */
typedef uint8_t TileMove;  

typedef struct
{
    MoveDirection moveActive;
    TileMove moves[8][8];
    uint8_t size;
} MoveFrame;

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