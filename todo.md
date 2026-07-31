## Must have

TODO savefile for scores
TODO check score calculation (8x8 game had unusually high score)
TODO architecture: in grid_move(): calculate completed move, then return MoveEvent containing vectors of tiles
     runGame() {
        ....
        moveEvent = grid_move(&grid);
        moveEvent += grid_newCell(&grid);
        // this needs to be figure out. i dont want to call grid_newCell() inside mrid_move()
        // maybe grid_newCell(&grid, &moveEvent)

        rendergrid(moveEvent)
     }

     then moveEvent could be something like
     struct {
        uint8_t dx[8][8];
        uint8_t dy[8][8]; // could be simplified by encoding dx and dy into a single uint8_t
        uint8_t merge[8][8]; // tell renderer to play merge animation and remove cell from memory
        uint8_t spawn[8][8]; // could be simplified by encoding x and y into a single uint8_t
     }


## Nice to have
TODO nicer background\
TODO sound\
TODO animations\
TODO bigger tiles for 4x4?\
TODO gbc?\


