#include "utest.h"
UTEST_MAIN()

#include "core/grid.h"

MoveFrame mf;
Grid grid;

UTEST(grid, initTest)
{
    grid_init(&grid);
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            ASSERT_EQ(grid.cells[y][x], 0);
        }
    }
}

UTEST(grid, pushSingleTest)
{
    grid_init(&grid);
    grid.cells[3][0] = 1;
    grid_prepare(&grid);
    while(grid_move(&grid, MOVE_UP).moveActive){};
    ASSERT_EQ(grid.cells[3][0], 0);
    ASSERT_EQ(grid.cells[0][0], 1);

    grid_prepare(&grid);
    while(grid_move(&grid, MOVE_RIGHT).moveActive){};
    ASSERT_EQ(grid.cells[0][0], 0);
    ASSERT_EQ(grid.cells[0][3], 1);

    grid_prepare(&grid);
    while(grid_move(&grid, MOVE_DOWN).moveActive){};
    ASSERT_EQ(grid.cells[0][3], 0);
    ASSERT_EQ(grid.cells[3][3], 1);

    grid_prepare(&grid);
    while(grid_move(&grid, MOVE_LEFT).moveActive){};
    ASSERT_EQ(grid.cells[3][3], 0);
    ASSERT_EQ(grid.cells[3][0], 1);
}

UTEST(grid, pushTwoTest)
{
    grid_init(&grid);
    grid.cells[3][0] = 1;
    grid.cells[0][0] = 1;

    grid_prepare(&grid);
    while(grid_move(&grid, MOVE_UP).moveActive){};
    ASSERT_EQ(grid.cells[3][0], 0);
    ASSERT_EQ(grid.cells[0][0], 2);

    grid_prepare(&grid);
    grid.cells[0][3] = 1;
    while(grid_move(&grid, MOVE_RIGHT).moveActive){};
    ASSERT_EQ(grid.cells[0][0], 0);
    ASSERT_EQ(grid.cells[0][3], 1);
    ASSERT_EQ(grid.cells[0][2], 2);

    grid.cells[0][3] = 2;
    grid_prepare(&grid);
    while(grid_move(&grid, MOVE_LEFT).moveActive){};
    ASSERT_EQ(grid.cells[0][0], 3);
    ASSERT_EQ(grid.cells[0][3], 0);
    ASSERT_EQ(grid.cells[0][2], 0);
}

UTEST(grid, pushThreeTest)
{
    grid_init(&grid);
    grid.cells[0][0] = 1;
    grid.cells[0][1] = 1;
    grid.cells[0][2] = 2;
    grid.cells[0][3] = 0;

    grid_prepare(&grid);
    while(grid_move(&grid, MOVE_RIGHT).moveActive){};
    ASSERT_EQ(grid.cells[0][0], 0);
    ASSERT_EQ(grid.cells[0][1], 0);
    ASSERT_EQ(grid.cells[0][2], 2);
    ASSERT_EQ(grid.cells[0][3], 2);
}

UTEST(grid, pushFourTest)
{
    grid_init(&grid);
    grid.cells[0][0] = 1;
    grid.cells[0][1] = 1;
    grid.cells[0][2] = 1;
    grid.cells[0][3] = 1;

    grid_prepare(&grid);
    while(grid_move(&grid, MOVE_RIGHT).moveActive){};
    ASSERT_EQ(grid.cells[0][0], 0);
    ASSERT_EQ(grid.cells[0][1], 0);
    ASSERT_EQ(grid.cells[0][2], 2);
    ASSERT_EQ(grid.cells[0][3], 2);
}

UTEST(grid, newCellTest)
{
    grid_init(&grid);

    grid_newCell(&grid);
    uint8_t num_changed = 0;
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if(grid.cells[y][x] != 0) {
                num_changed += 1;
            }
        }
    }
    ASSERT_EQ(num_changed, 1);

    grid_newCell(&grid);
    num_changed = 0;
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if(grid.cells[y][x] != 0) {
                num_changed += 1;
            }
        }
    }
    ASSERT_EQ(num_changed, 2);
}

UTEST(grid, moveframeTest) {
    grid_init(&grid);
    grid.cells[3][0] = 1;
    
    grid_prepare(&grid);

    mf = grid_move(&grid, MOVE_UP);

    ASSERT_EQ(mf.cells[3][0], 0);
    ASSERT_EQ(mf.cells[2][0], 1);
    ASSERT_EQ(mf.moveActive, MOVE_UP);
    
}
