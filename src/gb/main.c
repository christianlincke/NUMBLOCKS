// GBDK
#include <gb/gb.h>
#include <gbdk/console.h>
#include <gbdk/font.h>

// game stuff

#include "game.h"
#include "render.h"

void main(void)
{
    SHOW_BKG;

    startScreen();
    loadAssets();

    while(1){
        uint8_t gridSize = mainMenu();
        uint16_t score = runGame(gridSize);
    }
    
}
