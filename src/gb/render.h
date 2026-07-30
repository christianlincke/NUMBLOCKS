#pragma once

#include "core/moveframe.h"

typedef struct {
    uint8_t frame[GRID_SIZE][GRID_SIZE];
} Renderer;

/**
 * @brief draw empty grid
 * 
 */
void rendererBegin(Renderer *renderer);

/**
 * @brief draw new frame. only draws the difference
 * 
 * @param frame 
 */
void renderGrid(Renderer *renderer, MoveFrame *frame);