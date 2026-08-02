#pragma once

#include "core/moveframe.h"

#include "core/grid.h"
#include "core/menu.h"
#include "core/display.h"

typedef struct
{
    uint8_t size;

    MoveFrame* frame;
    Grid* grid;

    uint8_t animating;
    uint8_t animationFrame;
    uint8_t animationLength;

} Renderer;


void renderer_init(Renderer *renderer, Grid *grid, MoveFrame *frame);

void renderer_startAnimation(Renderer *renderer);

void renderer_update(Renderer *renderer);

void renderer_draw(Renderer *renderer);

void renderer_startGameOverAnimation(Renderer *renderer);

void renderMenu(Menu *menu, uint8_t x, uint8_t y);

void renderScore(uint16_t score);