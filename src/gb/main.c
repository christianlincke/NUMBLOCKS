#include <gb/gb.h>
#include <stdint.h>
#include <stdio.h>
#include <gbdk/console.h>

#include "core/add.h"


void main(void)
{
    int result = add(2, 3);
    printf("2 + 3 = %d\n", result);
    while (1)
    {
        vsync();
    }
}
