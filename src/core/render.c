#include "render.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GAME_OVER_ANIMATION_SPEED 30 // vsyncs per tile

void renderer_init(Renderer *renderer, Grid *grid, MoveFrame *frame)
{
    renderer->grid = grid;
    renderer->size = renderer->grid->size;
    renderer->frame = frame;

    renderer->animationFrame = 0;
    renderer->animationLength = 2;
    renderer->animating = 0;

    clearScreen();
    showBackground();
}

void renderer_startAnimation(Renderer *renderer)
{
    renderer->animationFrame = 0;
    renderer->animationLength = 2;
    renderer->animating = 1;
}

void renderer_update(Renderer *renderer)
{
    if (!renderer->animating)
        return;

    renderer->animationFrame++;

    if (renderer->animationFrame >= renderer->animationLength)
    {
        renderer->animating = 0;
    }
}

void renderer_draw(Renderer *renderer)
{
    for (int y = 0; y < renderer->size; y++)
    {
        for (int x = 0; x < renderer->size; x++)
        {
            // int drawX = x;
            // int drawY = y;

            // TileMove *tile = &renderer->frame.moves[y][x];
            // if (renderer->animating)
            // {
            //     drawX += tile->dx * renderer->animationFrame;
            //     drawY += tile->dy * renderer->animationFrame;
            // }

            // drawTile(renderer->size, drawX, drawY, renderer->frame.cells[y][x]);
            drawTile(renderer->size, x, y, renderer->grid->cells[y][x]);
        }
    }
}

void renderGameOverAnimation(Renderer *renderer)
{
    moveFrame_clear(renderer->frame);
    renderer->grid->cells[(uint8_t)(renderer->size - 2) / 2][(uint8_t)(renderer->size - 4) / 2] =
        16; // G
    renderer->grid
        ->cells[(uint8_t)(renderer->size - 2) / 2][(uint8_t)1 + (renderer->size - 4) / 2] = 17; // A
    renderer->grid
        ->cells[(uint8_t)(renderer->size - 2) / 2][(uint8_t)2 + (renderer->size - 4) / 2] = 18; // M
    renderer->grid
        ->cells[(uint8_t)(renderer->size - 2) / 2][(uint8_t)3 + (renderer->size - 4) / 2] = 19; // A

    renderer->grid
        ->cells[(uint8_t)1 + (renderer->size - 2) / 2][(uint8_t)(renderer->size - 4) / 2] = 20; // O
    renderer->grid
        ->cells[(uint8_t)1 + (renderer->size - 2) / 2][(uint8_t)1 + (renderer->size - 4) / 2] =
        21; // V
    renderer->grid
        ->cells[(uint8_t)1 + (renderer->size - 2) / 2][(uint8_t)2 + (renderer->size - 4) / 2] =
        22; // E
    renderer->grid
        ->cells[(uint8_t)1 + (renderer->size - 2) / 2][(uint8_t)3 + (renderer->size - 4) / 2] =
        23; // R

    renderer->animationFrame = 0;
    renderer->animationLength = 2;
    renderer->animating = 1;
}

void renderMenu(Menu *menu, uint8_t x, uint8_t y)
{
    for (uint8_t i = 0; i < menu->count; i++)
    {
        char optionString[20];

        if (menu->index == i)
        {
            sprintf(optionString, ">%s", menu->options[i]);
        }
        else
        {
            sprintf(optionString, " %s", menu->options[i]);
        }

        // Ensure null termination
        optionString[sizeof(optionString) - 1] = '\0';

        printString(x, y + i, optionString);
    }
}

void renderScore(uint16_t score)
{
    char scoreString[18];
    sprintf(scoreString, "Score: %d", score);
    scoreString[sizeof(scoreString) - 1] = '\0';
    printString(0, 17, scoreString);
}

void renderChecks(uint16_t checks)
{
    char checkString[18];
    sprintf(checkString, "ChecKs: %d", checks);
    checkString[sizeof(checkString) - 1] = '\0';
    printString(0, 0, checkString);
}