#pragma once

#include <stdint.h>

typedef struct {
    const char **options;
    const uint8_t *values;
    uint8_t count;
    uint8_t index;
} Menu;

uint8_t mainMenu();

uint8_t gameMenu();