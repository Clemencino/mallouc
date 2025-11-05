#ifndef BUCKET_H
#define BUCKET_H

struct free_list
{
	struct free_list *next;
};

struct bucket 
{
    size_t block_size;
    size_t capacity;
    void *chunk;
    void *free;
};

#endif /* ! BUCKET_H */
