// GBDK
#include <gb/gb.h>
#include <gbdk/console.h>
#include <gbdk/font.h>

// game stuff

#include "game.h"

#define GRID_SIZE 4

void main(void)
{
    SHOW_BKG;
    startScreen();
    while(1){
        uint16_t score = runGame(GRID_SIZE);
    }
    
}
