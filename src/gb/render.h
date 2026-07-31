#pragma once

#include "core/moveframe.h"
#include "menu.h"

typedef struct
{
    uint8_t size;

    MoveFrame frame;

    uint8_t animating;
    uint8_t animationFrame;
    uint8_t animationLength;

} Renderer;


void loadAssets();

void renderer_init(Renderer *renderer, uint8_t gridSize, MoveFrame *frame);

void renderer_startAnimation(Renderer *renderer, MoveFrame *frame);

void renderer_update(Renderer *renderer);

void renderer_draw(Renderer *renderer);

void renderer_drawNewCells(Renderer *renderer, MoveFrame *frame);

void renderGameOverAnimation(Renderer *renderer);

void renderStartScreen();

void renderScore(uint16_t score);

void renderMenu(Menu *menu, uint8_t x, uint8_t y);