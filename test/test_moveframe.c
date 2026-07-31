#include "utest.h"

#include "core/grid.h"

Grid grid;
MoveFrame frame;

UTEST(MoveFrame, moveframeTest)
{
    grid_init(&grid, 4);
    grid.cells[3][0] = 1;

    grid_prepare(&grid);

    grid_move(&grid, &frame, MOVE_UP);

    ASSERT_EQ(frame.cells[3][0], 0);
    ASSERT_EQ(frame.cells[2][0], 1);
    ASSERT_EQ(frame.moveActive, MOVE_UP);
}

UTEST(MoveFrame, moveframeUpdateTest)
{
    grid_init(&grid, 4);
    grid.cells[3][0] = 1;

    grid_prepare(&grid);

    grid_move(&grid, &frame, MOVE_UP);

    ASSERT_EQ(frame.cells[3][0], 0);
    ASSERT_EQ(frame.cells[2][0], 1);
    ASSERT_EQ(frame.moveActive, MOVE_UP);
}