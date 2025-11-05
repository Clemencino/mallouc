#include "alignment.h"

#include <stdio.h>

size_t align(size_t size)
{
    if (size % sizeof(long double) == 0)
    {
        return size;
    }
    size_t add = size % sizeof(long double);
    size_t add2 = sizeof(long double) - add;
    size_t res;
    if (__builtin_add_overflow(size, add2, &res))
    {
        return 0;
    }
    return res;
}
