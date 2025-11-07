#ifndef BUCKET_H
#define BUCKET_H

#include <stddef.h>
#include <sys/mman.h>
#include <unistd.h>

struct free_list
{
    struct free_list *next;
};

struct bucket
{
    size_t block_size;
    size_t capacity;
    void *chunk;
    size_t used;
    struct free_list *free;
    struct bucket *next;
};

struct bucket *bucket_find(size_t block_size);
struct bucket *bucket_new(size_t block_size);
void *bucket_alloc(struct bucket *b, size_t block_size);
void bucket_free_block(struct bucket *b, void *ptr);
void *page_begin(void *ptr);

#endif /* ! BUCKET_H */
