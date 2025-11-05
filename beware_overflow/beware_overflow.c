#include <stddef.h>

void *beware_overflow(void *ptr, size_t nmemb, size_t size)
{
    size_t tot = 0;
    if (__builtin_mul_overflow(nmemb, size, &tot))
    {
        return NULL;
    }
    char *ret = ptr;
    return ret + tot;
}

