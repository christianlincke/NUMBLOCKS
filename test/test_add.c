#include "utest.h"
UTEST_MAIN()

#include "core/add.h"

UTEST(lfo, lfo_init_test)
{
    int result = add(2, 3);
    ASSERT_EQ(result, 5);
}
