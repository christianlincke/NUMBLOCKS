#pragma once

#include <stdint.h>

// joypad defines for cli target
#ifdef TARGET_CLI
#define J_RIGHT 0x01
#define J_LEFT 0x02
#define J_UP 0x04
#define J_DOWN 0x08
#define J_A 0x10
#define J_B 0x20
#define J_SELECT 0x30
#define J_START 0x40
#endif

uint8_t joypadDebounce(uint8_t *jMem);