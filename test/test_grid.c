#include "utest.h"
UTEST_MAIN()

#include "core/grid.h"

MoveFrame mf;
Grid grid;

UTEST(grid, initTest)
{
    grid_init(&grid, 4);
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
    // init a single one at x = 0 y = 3
    grid_init(&grid, 4);
    grid.cells[3][0] = 1;

    // move up
    grid_prepare(&grid);
    moveFrame_clear(&mf);
    grid_move(&grid, &mf, MOVE_UP);
    ASSERT_EQ(grid.cells[3][0], 0);
    ASSERT_EQ(grid.cells[0][0], 1);

    // move right
    grid_prepare(&grid);
    moveFrame_clear(&mf);
    grid_move(&grid, &mf, MOVE_RIGHT);
    ASSERT_EQ(grid.cells[0][0], 0);
    ASSERT_EQ(grid.cells[0][3], 1);

    // move down
    grid_prepare(&grid);
    moveFrame_clear(&mf);
    grid_move(&grid, &mf, MOVE_DOWN);
    ASSERT_EQ(grid.cells[0][3], 0);
    ASSERT_EQ(grid.cells[3][3], 1);

    // move left
    grid_prepare(&grid);
    moveFrame_clear(&mf);
    grid_move(&grid, &mf, MOVE_LEFT);
    ASSERT_EQ(grid.cells[3][3], 0);
    ASSERT_EQ(grid.cells[3][0], 1);
}

UTEST(grid, pushTwoTest)
{
    grid_init(&grid, 4);
    grid.cells[3][0] = 1;
    grid.cells[0][0] = 1;

    grid_prepare(&grid);
    moveFrame_clear(&mf);

    grid_move(&grid, &mf, MOVE_UP);

    ASSERT_EQ(grid.cells[3][0], 0);
    ASSERT_EQ(grid.cells[0][0], 2);

    grid_prepare(&grid);
    moveFrame_clear(&mf);
    grid.cells[0][3] = 1;

    grid_move(&grid, &mf, MOVE_RIGHT);

    ASSERT_EQ(grid.cells[0][0], 0);
    ASSERT_EQ(grid.cells[0][3], 1);
    ASSERT_EQ(grid.cells[0][2], 2);

    grid.cells[0][3] = 2;
    grid_prepare(&grid);
    moveFrame_clear(&mf);

    grid_move(&grid, &mf, MOVE_LEFT);

    ASSERT_EQ(grid.cells[0][0], 3);
    ASSERT_EQ(grid.cells[0][3], 0);
    ASSERT_EQ(grid.cells[0][2], 0);
}

UTEST(grid, pushThreeTest)
{
    grid_init(&grid, 4);
    grid.cells[0][0] = 1;
    grid.cells[0][1] = 1;
    grid.cells[0][2] = 2;
    grid.cells[0][3] = 0;

    grid_prepare(&grid);
    moveFrame_clear(&mf);

    grid_move(&grid, &mf, MOVE_RIGHT);

    ASSERT_EQ(grid.cells[0][0], 0);
    ASSERT_EQ(grid.cells[0][1], 0);
    ASSERT_EQ(grid.cells[0][2], 2);
    ASSERT_EQ(grid.cells[0][3], 2);
}

UTEST(grid, pushFourTest)
{
    grid_init(&grid, 4);
    grid.cells[0][0] = 1;
    grid.cells[0][1] = 1;
    grid.cells[0][2] = 1;
    grid.cells[0][3] = 1;

    grid_prepare(&grid);
    moveFrame_clear(&mf);
    grid_move(&grid, &mf, MOVE_RIGHT);
    ASSERT_EQ(grid.cells[0][0], 0);
    ASSERT_EQ(grid.cells[0][1], 0);
    ASSERT_EQ(grid.cells[0][2], 2);
    ASSERT_EQ(grid.cells[0][3], 2);
}

UTEST(grid, newCellTest)
{
    grid_init(&grid, 4);

    grid_newCell(&grid, &mf, 1);
    uint8_t num_changed = 0;
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (grid.cells[y][x] != 0)
            {
                num_changed += 1;
            }
        }
    }
    ASSERT_EQ(num_changed, 1);

    grid_newCell(&grid, &mf, 2);
    num_changed = 0;
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (grid.cells[y][x] != 0)
            {
                num_changed += 1;
            }
        }
    }
    ASSERT_EQ(num_changed, 3);
}

UTEST(grid, GameOverTest)
{
    grid_init(&grid, 4);
    for (uint8_t i = 0; i < 16; i++)
    {
        uint8_t x = i % 4;
        uint8_t y = (uint8_t)i / 4;
        grid.cells[y][x] = i + 1;
    }
    ASSERT_EQ(grid_checkGameOver(&grid), 1);
    grid.cells[0][0] = 2;
    ASSERT_EQ(grid_checkGameOver(&grid), 0);

    grid.cells[0][0] = 1;
    grid.cells[3][3] = 15;
    ASSERT_EQ(grid_checkGameOver(&grid), 0);
}
