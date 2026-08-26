#include "core/game.h"
#include "core/render.h"

int main()
{
    startScreen();
    loadAssets();

    while(1){
        uint8_t gridSize = 4;
        uint16_t score = runGame(gridSize);
    }
    
}