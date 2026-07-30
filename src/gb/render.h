#pragma once

#include "core/moveframe.h"

typedef struct {
    uint8_t frame[8][8];
    uint8_t size;
} Renderer;

/**
 * @brief load data and draw background
 * 
 */
void rendererBegin(Renderer *renderer, uint8_t gridSize);

/**
 * @brief draw new frame. only draws the difference
 * 
 * @param frame 
 */
void renderGrid(Renderer *renderer, MoveFrame *frame);

/**
 * @brief show the score on screen
 * 
 * @param score the score (sum of all tiles)
 */
void renderScore(uint16_t score);

/**
 * @brief render game over screen
 * 
 */
void renderGameOverAnimation(Renderer *renderer);

/**
 * @brief  renders the start screen
 * 
 */
void renderStartScreen();