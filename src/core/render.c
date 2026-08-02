#include "render.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GAME_OVER_ANIMATION_SPEED 30 // vsyncs per tile

void renderer_init(Renderer *renderer, uint8_t gridSize, MoveFrame *frame)
{
    renderer->size = gridSize;
    renderer->frame = *frame;
    clearScreen();
    showBackground();
}

void renderer_startAnimation(Renderer *renderer, MoveFrame *frame)
{
    renderer->frame = *frame;

    renderer->animationFrame = 0;
    renderer->animationLength = 8;
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

            TileMove *tile = &renderer->frame.moves[y][x];

            int drawX = x;
            int drawY = y;

            if (renderer->animating)
            {
                drawX += tile->dx * renderer->animationFrame / renderer->animationLength;
                drawY += tile->dy * renderer->animationFrame / renderer->animationLength;
            }

            drawTile(renderer->size, drawX, drawY, renderer->frame.cells[y][x]);
        }
    }
}

void renderer_drawNewCells(Renderer *renderer, MoveFrame *frame)
{
    for (int y = 0; y < renderer->size; y++)
    {
        for (int x = 0; x < renderer->size; x++)
        {
            uint8_t tile = frame->cells[y][x];
            if (tile != 0)
            {
                drawTile(renderer->size, x, y, tile);
            }
        }
    }
}

/*
void renderGameOverAnimation(Renderer *renderer)
{
    uint8_t startTile;
    uint8_t counter;
    uint8_t tileMap0[4] = {0, 2, 1, 3};

    // before we do anything else, lets wait for a second
    counter = 0;
    while (counter < 60)
    {
        counter++;
        vsync();
    }
    counter = 0;

    // first line all empty
    for (int y = 0; y < renderer->size; y++)
    {
        for (int x = 0; x < renderer->size; x++)
        {
            uint8_t bx = x * 2 + ((20 - 2 * renderer->size) / 2); // SCREENWIDTH in gb tiles
            uint8_t by = y * 2 + ((18 - 2 * renderer->size) / 2); // SCREENHEIGHT in gb tiles

            if (y == 1)
            {
                startTile = 0x40;
                uint8_t tileMap[] = {x * 4 + startTile + 0, x * 4 + startTile + 2,
                                     x * 4 + startTile + 1, x * 4 + startTile + 3};
                set_bkg_tiles(bx, by, 2, 2, tileMap);
            }
            else if (y == 2)
            {
                startTile = 0x50;
                uint8_t tileMap[] = {x * 4 + startTile + 0, x * 4 + startTile + 2,
                                     x * 4 + startTile + 1, x * 4 + startTile + 3};
                set_bkg_tiles(bx, by, 2, 2, tileMap);
            }
            else
            {
                uint8_t tileMap0[] = {0, 2, 1, 3};
                set_bkg_tiles(bx, by, 2, 2, tileMap0);
            }

            counter = 0;
            while (counter < GAME_OVER_ANIMATION_SPEED)
            {
                counter++;
                vsync();
            }
        }
    }
}
    */

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
