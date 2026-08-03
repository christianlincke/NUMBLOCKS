#include <gbdk/platform.h>
#include <stdint.h>
#include <stdio.h>

#pragma dataseg DATA_0  // Sets Cart SRAM bank to 0

uint16_t sram_magic;
uint16_t sram_scores[5];