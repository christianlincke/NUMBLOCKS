## Must have

TODO menu (start new game)\
TODO calculate score\
TODO savefile for scores


## Nice to have
TODO nicer background\
TODO sound\
TODO animations\
TODO grid size - this already works by changing GRID_SIZE macro \
TODO bigger tiles for 4x4?\
TODO gbc?\



TODO clean implementation:
int main() {
    startScreen();
    ...

    while(true){
        int gridSize = gameMenu();
        int score = runGame(gridSize);
    }
}

int runGame(gridSize) {
    Grid grid;
    grid_init(&grid, gridSize); (-> grid.size = gridSize)
}


