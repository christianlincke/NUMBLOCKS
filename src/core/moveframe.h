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
    uint8_t value[GRID_SIZE][GRID_SIZE];
    int8_t dx[GRID_SIZE][GRID_SIZE];
    int8_t dy[GRID_SIZE][GRID_SIZE];
    uint8_t merged[GRID_SIZE][GRID_SIZE];
} MoveFrame;

/**
 * @brief initialize / clear the move event
 *
 * @param event
 */
void moveFrame_clear(MoveFrame *event);