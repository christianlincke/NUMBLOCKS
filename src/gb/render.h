#pragma once

#include "core/moveframe.h"
#include "menu.h"

typedef struct {
    uint8_t frame[8][8];
    uint8_t size;
} Renderer;


void loadAssets();

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
 * @brief render game over screen
 * 
 */
void renderGameOverAnimation(Renderer *renderer);

/**
 * @brief  renders the start screen
 * 
 */
void renderStartScreen();

/**
 * @brief show the score on screen
 * 
 * @param score the score (sum of all tiles)
 */
void renderScore(Renderer *renderer, uint16_t score);

void renderMenu(Menu *menu, uint8_t x, uint8_t y);