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

#include "textbuffer.h"
#include "moveframe.h"
#include "defines.h"

#include <stdint.h>


/**
 * @brief this is where the core data lives
 * 
 */
typedef struct {
    uint8_t cells[GRID_SIZE][GRID_SIZE];
    uint8_t aux[GRID_SIZE][GRID_SIZE]; // TODO does this live here?
} Grid;

/**
 * @brief initialize grid
 * 
 * @param grid 
 */
void grid_init(Grid *grid);

/**
 * @brief prepare for a new move
 * 
 * @param grid 
 */
void grid_prepare(Grid *grid);

/**
 * @brief push all cells in the grid into direction dir
 * 
 * @param grid grid
 * @param dir direction
 * @return uint8_t 0 if push is completed, dir if there might be more to do
 */
MoveFrame grid_move(Grid *grid, MoveDirection dir);

/**
 * @brief adds an randomly selected empty sell with a random value (2 or 4)
 *
 * @param grid the grid
 */
MoveFrame grid_newCell(Grid *grid);

/**
 * @brief dump grid content into textbuffer
 *
 * @param grid
 * @param buffer
 */
void grid_dump(Grid *grid, TextBuffer *buffer);

/**
 * @brief calculate the sum of all cells
 * 
 * @param grid 
 * @return int16_t 
 */
uint16_t grid_sumCells(Grid *grid);