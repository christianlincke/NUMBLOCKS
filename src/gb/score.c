#include "core/score.h"

#include <gbdk/platform.h>

extern uint16_t sram_magic;     // in sram bank 0
extern uint16_t sram_scores[5]; // in sram bank 0

Scores loadScores()
{
    ENABLE_RAM;
    SWITCH_RAM(0);
    if (sram_magic != 0x2048)
    {
        sram_scores[0] = 0;
        sram_scores[1] = 0;
        sram_scores[2] = 0;
        sram_scores[3] = 0;
        sram_scores[4] = 0;
        sram_magic = 0x2048;
    }

    Scores scores;
    for (int i = 0; i < 5; i++)
    {
        scores.scores[i] = sram_scores[i];
    }
    DISABLE_RAM_MBC5;
    
    return scores;
}

void saveScore(uint8_t gridSize, uint16_t score)
{
    ENABLE_RAM;
    SWITCH_RAM(0);
    sram_scores[gridSize - 4] = score;
    DISABLE_RAM_MBC5;
}