#pragma once

#include <stdint.h>

typedef struct {
    uint16_t scores[5];
} Scores;

Scores loadScores();

void saveScore(uint8_t gridSize, uint16_t score);