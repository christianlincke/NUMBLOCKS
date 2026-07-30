#include "joypad.h"

#include <gb/gb.h>

uint8_t joypadDebounce(uint8_t *jMem)
{
    uint8_t j = joypad();
    if (j == *jMem)
    {
        return 0;
    }
    *jMem = j;
    return j;
}