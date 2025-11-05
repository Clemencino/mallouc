#include "page_begin.h"

void *page_begin(void *ptr, size_t page_size)
{
    char *p = ptr;
    size_t pp = (size_t)p;
    size_t mask = page_size - 1;
    size_t alt = pp & mask;
    return (p - alt);
}
