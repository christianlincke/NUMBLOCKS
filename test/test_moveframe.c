#include "utest.h"

#include "core/grid.h"

Grid grid;
MoveFrame frame;

UTEST(MoveFrame, singleMoveFrameTest)
{
    Grid grid;
    MoveFrame frame;

    grid_init(&grid, 4);
    grid.cells[3][0] = 1;

    grid_prepare(&grid);

    grid_move(&grid, &frame, MOVE_UP);

    TileMove move = frame.moves[3][0];
    ASSERT_EQ(move.dx, 0);
    ASSERT_EQ(move.dy, -3);
    ASSERT_EQ(move.value, 1);

    grid_init(&grid, 4);
    grid.cells[2][2] = 1;
    grid_prepare(&grid);
    grid_move(&grid, &frame, MOVE_LEFT);
    move = frame.moves[2][2];
    ASSERT_EQ(move.dx, -2);
    ASSERT_EQ(move.dy, 0);
    ASSERT_EQ(move.value, 1);

    grid_init(&grid, 4);
    grid.cells[3][3] = 4;
    grid_prepare(&grid);
    grid_move(&grid, &frame, MOVE_DOWN);
    move = frame.moves[3][3];
    ASSERT_EQ(move.dx, 0);
    ASSERT_EQ(move.dy, 0);

    ASSERT_EQ(frame.cells[3][3], 4);
}

UTEST(MoveFrame, mergeFrameTest)
{
    Grid grid;
    MoveFrame frame;
    grid_init(&grid, 4);
    grid.cells[0][2] = 1;
    grid.cells[0][0] = 1;
    grid_prepare(&grid);
    grid_move(&grid, &frame, MOVE_LEFT);
    TileMove move = frame.moves[0][2];
    ASSERT_EQ(move.dx, -2);
    ASSERT_EQ(move.dy, 0);
    ASSERT_EQ(move.merge, 0xF0);


    ASSERT_EQ(frame.cells[0][0], 1);
    ASSERT_EQ(frame.moves[0][0].merge, 0x0F);
}

UTEST(MoveFrame, noMoveFrameTest)
{
    Grid grid;
    MoveFrame frame;
    grid_init(&grid, 4);
    grid.cells[0][0] = 1;
    grid.cells[0][1] = 2;
    grid.cells[0][2] = 3;

    grid_prepare(&grid);
    grid_move(&grid, &frame, MOVE_LEFT);

    ASSERT_EQ(frame.cells[0][0], 1);
    ASSERT_EQ(frame.moves[0][0].merge, 0);
    ASSERT_EQ(frame.moves[0][0].dx, 0);
    ASSERT_EQ(frame.moves[0][0].dy, 0);

    ASSERT_EQ(frame.cells[0][1], 2);
    ASSERT_EQ(frame.moves[0][1].merge, 0);
    ASSERT_EQ(frame.moves[0][1].dx, 0);
    ASSERT_EQ(frame.moves[0][1].dy, 0);

    ASSERT_EQ(frame.cells[0][2], 3);
    ASSERT_EQ(frame.moves[0][2].merge, 0);
    ASSERT_EQ(frame.moves[0][2].dx, 0);
    ASSERT_EQ(frame.moves[0][2].dy, 0);
}

