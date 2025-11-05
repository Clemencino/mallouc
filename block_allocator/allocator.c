#include "allocator.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

allocator.o main.o utils.o -o allocatortruct blk_allocator *blka_new(void)
{
    struct blk_allocator *new = malloc(sizeof(struct blk_allocator));
    if (!new)
    {
        return NULL;
    }
    new->meta = NULL;
    return new;
}

struct blk_meta *blka_alloc(struct blk_allocator *blka, size_t size)
{
    long page_size = sysconf(_SC_PAGESIZE);
    size_t total_size = sizeof(struct blk_meta) + size;
    size_t pages = (total_size + page_size - 1) / page_size;
    size_t total_alloc = pages * page_size;

    struct blk_meta *meta = mmap(NULL, total_alloc, PROT_READ | PROT_WRITE,
                                 MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (meta == MAP_FAILED)
        return NULL;

    meta->size = total_alloc - sizeof(struct blk_meta);
    meta->next = blka->meta;
    blka->meta = meta;

    return meta;
}

void blka_free(struct blk_meta *blk)
{
    size_t total_size = blk->size + sizeof(struct blk_meta);
    munmap(blk, total_size);
}

void blka_pop(struct blk_allocator *blka)
{
    if (!blka->meta)
    {
        return;
    }
    struct blk_meta *to_free = blka->meta;
    blka->meta = to_free->next;
    blka_free(to_free);
}

void blka_delete(struct blk_allocator *blka)
{
    while (blka->meta)
    {
        blka_pop(blka);
    }
    free(blka);
}
