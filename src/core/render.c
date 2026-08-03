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

    renderer->gameOverFrameIndex = 0;
    renderer->gameOverTileIndex = 0;

    clearScreen();
    showBackground();
}

void renderer_takeSnapshot(Renderer *renderer)
{
    memcpy(renderer->snapshotGrid, renderer->grid->cells, 64);
}

void renderer_startAnimation(Renderer *renderer, Animation animation)
{
    renderer->animation = animation;
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
        if (renderer->animation == ANIMATION_GAMEOVER)
        {
            renderer->animationFrame = 0;
            renderer_updateGameOverAnimation(renderer);
        }
        else
        {
            renderer->animating = 0;
        }
    }
}

void renderer_drawAll(Renderer *renderer)
{
    for (int y = 0; y < renderer->size; y++)
    {
        for (int x = 0; x < renderer->size; x++)
        {
            drawTile(renderer->size, x, y, renderer->grid->cells[y][x]);
        }
    }
}

void renderer_drawDiff(Renderer *renderer)
{
    for (int y = 0; y < renderer->size; y++)
    {
        for (int x = 0; x < renderer->size; x++)
        {
            if (renderer->snapshotGrid[y][x] != renderer->grid->cells[y][x])
            {
                if (renderer->animation != ANIMATION_GAMEOVER)
                {
                    drawTile(renderer->size, x, y, renderer->grid->cells[y][x]);
                }
                else
                {
                    drawTile(renderer->size, x, y, renderer->snapshotGrid[y][x]);
                }
            }
        }
    }
}

void renderer_updateGameOverAnimation(Renderer *renderer)
{
    uint8_t x = renderer->gameOverFrameIndex % renderer->size;
    uint8_t y = (uint8_t)renderer->gameOverFrameIndex / renderer->size;

    uint8_t startX = (uint8_t)(renderer->size - 4) / 2;
    uint8_t endX = startX + 3;
    uint8_t startY = (uint8_t)(renderer->size - 2) / 2;
    uint8_t endY = startY + 1;

    if (x >= startX && x <= endX && y == startY)
    {
        renderer->snapshotGrid[y][x] = renderer->gameOverTileIndex + 16;
        renderer->gameOverTileIndex++;
    }
    else if (x >= startX && x <= endX && y == endY)
    {
        renderer->snapshotGrid[y][x] = renderer->gameOverTileIndex + 16;
        renderer->gameOverTileIndex++;
    }
    else
    {
        renderer->snapshotGrid[y][x] = 0;
    }
    renderer->gameOverFrameIndex++;

    if (renderer->gameOverFrameIndex == renderer->size * renderer->size)
    {
        renderer->animating = 0;
    }
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

void renderTimestamp(uint16_t timestamp)
{
    char checkString[18];
    sprintf(checkString, "Time: %d", timestamp);
    checkString[sizeof(checkString) - 1] = '\0';
    printString(0, 0, checkString);
}