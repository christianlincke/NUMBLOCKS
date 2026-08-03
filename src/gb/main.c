#include "core/game.h"
#include "core/render.h"
#include "core/score.h"

void main(void)
{
    Scores scores = loadScores();

    startScreen();
    loadAssets();

    while (1)
    {
        clearScreen();
        uint8_t mainSelection = mainMenu();
        if (mainSelection == 1)
        {
            showScores(scores);
        }
        else
        {
            uint8_t gridSize = mainSelection;
            uint16_t score = runGame(gridSize);
            if (score > scores.scores[gridSize - 4])
            {
                scores.scores[gridSize - 4] = score;
                saveScore(gridSize, score);
            }
        }
    }
}
