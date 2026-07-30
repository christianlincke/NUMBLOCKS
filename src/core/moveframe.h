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

#include "defines.h"

#include <stdint.h>

typedef struct
{
    uint8_t cells[GRID_SIZE][GRID_SIZE];
    MoveDirection moveActive;
} MoveFrame;

/**
 * @brief initialize / clear the move event
 *
 * @param frame
 */
void moveFrame_clear(MoveFrame *frame);