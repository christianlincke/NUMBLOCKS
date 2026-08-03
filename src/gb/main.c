
#include "core/game.h"
#include "core/render.h"

void main(void)
{
    startScreen();
    loadAssets();

    while(1){
        clearScreen();
        uint8_t gridSize = mainMenu();
        uint16_t score = runGame(gridSize);
    }
    
}
