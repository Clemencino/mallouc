#include "malloc.h"

#include <stddef.h>
#include <string.h>
#include <unistd.h>

#include "bucket.h"

static size_t align8(size_t n)
{
    while (n % 8 != 0)
        n += 1;
    return n;
}

static size_t next_pow2(size_t n)
{
    size_t x = 8;
    while (x < n)
        x *= 2;
    return x;
}

static size_t page_size(void)
{
    static size_t sz = 0;
    if (sz == 0)
        sz = sysconf(_SC_PAGESIZE);
    return sz;
}

__attribute__((visibility("default"))) void *malloc(size_t size)
{
    if (size == 0)
        return NULL;
    size = align8(size);
    size_t block_size = next_pow2(size);

    struct bucket *b = bucket_find(block_size);
    if (!b)
        b = bucket_new(block_size);
    if (!b)
        return NULL;

    return bucket_alloc(b);
}

__attribute__((visibility("default"))) void free(void *ptr)
{
    if (!ptr)
        return;
    void *page = page_begin(ptr, page_size());
    struct bucket *b = (struct bucket *)page;
    bucket_free_block(b, ptr);
}

__attribute__((visibility("default"))) void *realloc(void *ptr, size_t size)
{
    if (!ptr)
        return malloc(size);
    if (size == 0)
    {
        free(ptr);
        return NULL;
    }
    void *page = page_begin(ptr, page_size());
    struct bucket *b = (struct bucket *)page;
    size_t old_size = b->block_size;

    void *new_ptr = malloc(size);
    if (!new_ptr)
        return NULL;
    size_t to_copy;
    if (old_size < size)
        to_copy = old_size;
    else
        to_copy = size;
    memcpy(new_ptr, ptr, to_copy);
    if (size > old_size)
        memset((char *)new_ptr + old_size, 0, size - old_size);

    free(ptr);
    return new_ptr;
}
__attribute__((visibility("default"))) void *calloc(size_t nmemb, size_t size)
{
    size_t total;
    if (__builtin_mul_overflow(nmemb, size, &total))
        return NULL;

    void *p = malloc(total);
    if (p)
        memset(p, 0, total);
    return p;
}

int main(void)
{
    return 0;
}
