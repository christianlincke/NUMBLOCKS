#include "utest.h"
UTEST_MAIN()

#include "core/grid.h"

UTEST(grid, grid_init_test)
{
    Grid grid;
    grid_init(&grid);
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            ASSERT_EQ(grid.cells[y][x], 0);
        }
    }
}

UTEST(grid, grid_push_Single_test)
{
    Grid grid;
    grid_init(&grid);
    grid.cells[3][0] = 1;
    grid_push(&grid, UP);
    ASSERT_EQ(grid.cells[3][0], 0);
    ASSERT_EQ(grid.cells[0][0], 1);

    grid_push(&grid, RIGHT);
    ASSERT_EQ(grid.cells[0][0], 0);
    ASSERT_EQ(grid.cells[0][3], 1);

    grid_push(&grid, DOWN);
    ASSERT_EQ(grid.cells[0][3], 0);
    ASSERT_EQ(grid.cells[3][3], 1);

    grid_push(&grid, LEFT);
    ASSERT_EQ(grid.cells[3][3], 0);
    ASSERT_EQ(grid.cells[3][0], 1);
}

UTEST(grid, grid_push_two_test)
{
    Grid grid;
    grid_init(&grid);
    grid.cells[3][0] = 1;
    grid.cells[0][0] = 1;
    
    grid_push(&grid, UP);
    ASSERT_EQ(grid.cells[3][0], 0);
    ASSERT_EQ(grid.cells[0][0], 2);
    
    grid.cells[0][3] = 1;
    grid_push(&grid, RIGHT);
    ASSERT_EQ(grid.cells[0][0], 0);
    ASSERT_EQ(grid.cells[0][3], 1);
    ASSERT_EQ(grid.cells[0][2], 2);

    grid.cells[0][3] = 2;
    grid_push(&grid, LEFT);
    ASSERT_EQ(grid.cells[0][0], 4);
    ASSERT_EQ(grid.cells[0][3], 0);
    ASSERT_EQ(grid.cells[0][2], 0);
}


