#include "core/random_byte.h"

#include <rand.h>

uint8_t random_byte(void)
{
    return (uint8_t)rand();
}