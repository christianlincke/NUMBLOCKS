#pragma once

#include <stdint.h>

void startScreen();

uint8_t mainMenu();

uint8_t gameMenu();

uint16_t runGame(uint8_t gridSize);