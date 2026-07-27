#include <stdio.h>

#include "core/add.h"

int main(int argc, char *argv[])
{
    int result = add(2, 3);
    printf("The sum of 2 and 3 is: %d\n", result);
    return 0;
}