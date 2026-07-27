#include "utest.h"
UTEST_MAIN()

#include "core/grid.h"

UTEST(grid, initTest)
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

UTEST(grid, pushSingleTest)
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

UTEST(grid, pushTwoTest)
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

UTEST(grid, pushThreeTest)
{
    Grid grid;
    grid_init(&grid);
    grid.cells[0][0] = 1;
    grid.cells[0][1] = 1;
    grid.cells[0][2] = 2;
    grid.cells[0][3] = 0;

    grid_push(&grid, RIGHT);
    ASSERT_EQ(grid.cells[0][0], 0);
    ASSERT_EQ(grid.cells[0][1], 0);
    ASSERT_EQ(grid.cells[0][2], 2);
    ASSERT_EQ(grid.cells[0][3], 2);
}

UTEST(grid, pushFourTest)
{
    Grid grid;
    grid_init(&grid);
    grid.cells[0][0] = 1;
    grid.cells[0][1] = 1;
    grid.cells[0][2] = 1;
    grid.cells[0][3] = 1;

    grid_push(&grid, RIGHT);
    ASSERT_EQ(grid.cells[0][0], 0);
    ASSERT_EQ(grid.cells[0][1], 0);
    ASSERT_EQ(grid.cells[0][2], 2);
    ASSERT_EQ(grid.cells[0][3], 2);
}

UTEST(grid, newCellTest)
{
    Grid grid;
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
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if(grid.cells[y][x] != 0) {
                num_changed += 2;
            }
        }
    }
}
