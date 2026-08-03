#pragma once

#include "core/score.h"

#include <stdint.h>

void startScreen();

void showScores(Scores scores);

uint8_t mainMenu();

uint8_t gameMenu();

uint16_t runGame(const uint8_t gridSize);