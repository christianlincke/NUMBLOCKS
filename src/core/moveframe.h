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
 * @brief 
 * @param merge 0 if there is no merge, 0x0F if tile is merged into, 0xF0 is tile moves and then merges into other tile
 */
typedef struct {
    int8_t dx;
    int8_t dy;
    uint8_t merge;
} TileMove;

typedef struct
{
    int8_t cells[8][8];
    MoveDirection moveActive; // probably delete late
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