#pragma once

#include <stdint.h>

void loadAssets();

uint8_t getTileAddress(const char c);

void drawTile(uint8_t gridSize, uint8_t x, uint8_t y, uint8_t value);

void showStartScreen();

void clearScreen();

void showBackground();

void renderScore(uint16_t score);

void printString(uint8_t x, uint8_t y, const char *text);