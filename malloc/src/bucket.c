#include "bucket.h"

#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

static struct bucket *g_buckets = NULL;

void *page_begin(void *ptr, size_t page_size)
{
    char *p = ptr;
    size_t pp = (size_t)p;
    size_t mask = page_size - 1;
    size_t alt = pp & mask;
    return (p - alt);
}

struct bucket *bucket_find(size_t block_size)
{
    struct bucket *b = g_buckets;
    while (b)
    {
        if (b->block_size == block_size)
            return b;
        b = b->next;
    }
    return NULL;
}

struct bucket *bucket_new(size_t block_size)
{
    long ps = sysconf(_SC_PAGESIZE);
    size_t blocks_per_page = ps / block_size;
    if (blocks_per_page == 0)
        blocks_per_page = 1;

    size_t total_size = blocks_per_page * block_size;
    void *mem =
        mmap(NULL, total_size + sizeof(struct bucket), PROT_READ | PROT_WRITE,
             MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (mem == MAP_FAILED)
        return NULL;

    struct bucket *b = mem;
    b->block_size = block_size;
    b->capacity = blocks_per_page;
    b->chunk = (char *)mem + sizeof(struct bucket);
    b->next = g_buckets;
    g_buckets = b;

    char *p = b->chunk;
    struct free_list *cur = (struct free_list *)p;
    b->free = cur;
    for (size_t i = 0; i < blocks_per_page - 1; i++)
    {
        struct free_list *next = (struct free_list *)(p + block_size);
        cur->next = next;
        cur = next;
        p += block_size;
    }
    cur->next = NULL;

    return b;
}

void *bucket_alloc(struct bucket *b)
{
    if (!b->free)
        return NULL;

    struct free_list *f = b->free;
    b->free = f->next;
    return (void *)f;
}

void bucket_free_block(struct bucket *b, void *ptr)
{
    if (!ptr)
        return;

    struct free_list *f = ptr;
    f->next = b->free;
    b->free = f;
}
