#pragma once

#include "core/moveframe.h"

#include "core/grid.h"
#include "core/menu.h"
#include "core/display.h"

typedef enum {
    ANIMATION_NONE = 0,
    ANIMATION_MOVE = 1,
    ANIMATION_NEWCELL = 2,
    ANIMATION_GAMEOVER = 3
} Animation;

typedef struct
{
    uint8_t size;

    MoveFrame* frame;
    Grid* grid;

    uint8_t snapshotGrid[8][8];
    Animation animation;

    uint8_t animating;
    uint8_t animationFrame;
    uint8_t animationLength;

} Renderer;


void renderer_init(Renderer *renderer, Grid *grid, MoveFrame *frame);

void renderer_takeSnapshot(Renderer *renderer);

void renderer_startAnimation(Renderer *renderer, Animation animation);

void renderer_update(Renderer *renderer);

void renderer_drawAll(Renderer *renderer);

void renderer_drawDiff(Renderer *renderer);

void renderer_updateGameOverAnimation(Renderer *renderer);

void renderMenu(Menu *menu, uint8_t x, uint8_t y);

void renderScore(uint16_t score);

/**
 * @brief print how many cells are checked during each move. for debug / optim
 * 
 * @param checks 
 */
void renderTimestamp(uint16_t timestamp);