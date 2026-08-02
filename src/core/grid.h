#pragma once
/**
 * @file grid.h
 * @author christian
 * @brief game core: grid and logic
 * @version 0.1
 * @date 2026-07-29
 *
 * @copyright Copyright (c) 2026
 *
 */

#include "moveframe.h"

#include <stdint.h>

typedef struct
{
    MoveDirection moveDir;

    int8_t dx;
    uint8_t startX;
    int8_t endX;
    int8_t stepX;

    int8_t dy;
    uint8_t startY;
    int8_t endY;
    int8_t stepY;
} MoveData;

/**
 * @brief this is where the core data lives
 *
 */
typedef struct
{
    uint8_t cells[8][8];
    uint8_t aux[8];
    MoveData moveData;
    MoveFrame *frame;
    uint8_t size;
} Grid;

void moveData_clear(MoveData *moveData);

/**
 * @brief initialize grid
 *
 * @param grid
 */
void grid_init(Grid *grid, MoveFrame *frame, uint8_t gridSize);

/**
 * @brief prepare for a new move
 *
 * @param grid
 */
void grid_prepare(Grid *grid, MoveDirection moveDir);

/**
 * @brief push all cells in the grid into direction dir
 *
 * @param grid grid
 * @param dir direction
 * @return uint8_t 0 if push is completed, dir if there might be more to do
 */
MoveDirection grid_move(Grid *grid);

/**
 * @brief adds an randomly selected empty sell with a random value (2 or 4)
 *
 * @param grid the grid
 */
void grid_newCell(Grid *grid, const uint8_t numNewCells);

/**
 * @brief calculate the sum of all cells
 *
 * @param grid
 * @return int16_t sum of all the cells
 */
uint16_t grid_sumCells(Grid *grid);

/**
 * @brief checks whether there are any possible moves
 *
 * @param grid
 * @return uint8_t 1 if game over
 */
uint8_t grid_checkGameOver(Grid *grid);