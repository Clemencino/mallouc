#include "bucket.h"

// all buckets
static struct bucket *g_buckets = NULL;

// find page begin
void *page_begin(void *ptr)
{/*
    char *p = ptr;
    size_t pp = (size_t)p;
    size_t mask = page_size - 1;
    size_t alt = pp & mask;
    return (p - alt);
    */
    struct bucket *b = g_buckets;
    char *p = ptr;
    while (b)
    {
        char *start = (char *)b->chunk;
        char *end = start + b->block_size * b->capacity;
        if (p >= start && p < end)
        {
            return b;
        }
        b = b->next;
    }
    return NULL;
}

// return the bucket of the size or NUll if not found
struct bucket *bucket_find(size_t block_size)
{
    struct bucket *buck = g_buckets;
    while (buck)
    {
        if (buck->block_size == block_size && buck->free)
        {
            return buck;
        }
        buck = buck->next;
    }
    return NULL;
}

// create a new bucket
struct bucket *bucket_new(size_t block_size)
{
    long page_size = sysconf(_SC_PAGESIZE);
    size_t blocks_per_page = page_size / block_size;

    if (blocks_per_page == 0)
    {
        blocks_per_page = 1; // always (at least)one block
    }

    size_t total_size = blocks_per_page * block_size + sizeof(struct bucket);
    void *mem = mmap(NULL, total_size, PROT_READ | PROT_WRITE,
                     MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (mem == MAP_FAILED)
    {
        return NULL;
    }

    struct bucket *b = mem;

    
    char *p = mem;
    p += sizeof(struct bucket);
    size_t rem = ((size_t)p) % sizeof(long double);
    if (rem != 0)
        p += sizeof(long double) - rem;
    b->chunk = p;
    b->block_size = block_size;
    b->capacity = blocks_per_page;
    b->used = 0;
    b->next = g_buckets;
    g_buckets = b;

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

// find free block
void *bucket_alloc(struct bucket *b, size_t block_size)
{
    struct free_list *f = NULL;
    if (!b->free)
    {
        struct bucket *new = bucket_new(block_size);
        f = new->free;
        new->free = f->next;
    }
    else
    {
        f = b->free;
        b->free =
        f->next;
    }
    b->used++;
    return (void *)f;
}

// block is free
void bucket_free_block(struct bucket *b, void *ptr)
{
    if (!ptr)
    {
        return;
    }
    struct free_list *f = ptr;
    f->next = b->free;
    b->free = f;
    b->used--;

    if (b->used == 0)
    {
        struct bucket *prev = NULL;
        struct bucket *cur = g_buckets;
        while (cur && cur != b)
        {
            prev = cur;
            cur = cur->next;
        }

        if (cur)
        {
            if (prev)
            {
                prev->next = cur->next;
            }
            else
            {
                g_buckets = cur->next;
            }

            munmap(cur, cur->capacity * cur->block_size + sizeof(struct bucket));
        }
    }
}
