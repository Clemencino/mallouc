#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/malloc.h"

#define N 5
#define NUM_SIZES 12

int is_aligned(void *ptr)
{
    size_t addr = (size_t)ptr;
    if (addr % sizeof(long double) == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void test_malloc()
{
    printf("Test malloc\n");
    size_t sizes[NUM_SIZES] = { 1, 2, 3, 4, 7, 8, 15, 16, 31, 32, 63, 64 };
    for (int i = 0; i < NUM_SIZES; i++)
    {
        size_t size = sizes[i];
        void *ptrs[N];
        for (int j = 0; j < N; j++)
        {
            ptrs[j] = malloc(size);
            if (!ptrs[j])
            {
                printf("malloc(%zu) returned NULL\n", size);
            }
            else
            {
                if (is_aligned(ptrs[j]))
                {
                    printf("malloc(%zu) ptr %d: %p aligned\n", size, j,
                           ptrs[j]);
                }
                else
                {
                    printf("malloc(%zu) ptr %d: %p not aligned\n", size, j,
                           ptrs[j]);
                }
            }
        }
        for (int j = 0; j < N; j++)
        {
            free(ptrs[j]);
        }
    }
}

void test_realloc()
{
    printf("Test realloc\n");
    size_t sizes[NUM_SIZES] = { 8, 16, 24, 32, 48, 64 };
    for (int i = 0; i < NUM_SIZES / 2; i++)
    {
        void *ptr = malloc(sizes[i]);
        if (!ptr)
        {
            continue;
        }
        if (is_aligned(ptr))
        {
            printf("Initial malloc %zu: %p aligned\n", sizes[i], ptr);
        }
        else
        {
            printf("Initial malloc %zu: %p not aligned\n", sizes[i], ptr);
        }

        size_t new_size = sizes[i] * 2;
        void *new_ptr = realloc(ptr, new_size);
        if (!new_ptr)
        {
            free(ptr);
            continue;
        }
        ptr = new_ptr;
        if (is_aligned(ptr))
        {
            printf("After realloc %zu: %p aligned\n", new_size, ptr);
        }
        else
        {
            printf("After realloc %zu: %p not aligned\n", new_size, ptr);
        }

        free(ptr);
    }
}

void test_free()
{
    printf("Test free\n");
    void *ptrs[NUM_SIZES];
    for (int i = 0; i < NUM_SIZES; i++)
    {
        ptrs[i] = malloc(i * 8 + 1);
        if (ptrs[i])
        {
            printf("malloc %d: %p\n", i * 8 + 1, ptrs[i]);
        }
    }

    for (int i = 0; i < NUM_SIZES; i++)
    {
        free(ptrs[i]);
        printf("freed malloc %d\n", i * 8 + 1);
    }
}

int main()
{
    test_malloc();
    test_realloc();
    test_free();
    return 0;
}
/*
int main()
{
    // printf("Malloc\n");
    char *p = malloc(15);
    if (!p)
    {
        return 1;
    }
    for (int i = 0; i < 15; i++)
    {
        p[i] = i;
    }
    for (int i = 0; i < 15; i++)
    {
        printf("%d ", p[i]);
    }
    printf("\n");
    free(p);

    //  printf("Calloc\n");
    char *q = calloc(3, 4);
    for (int i = 0; i < 12; i++)
    {
        printf("%d ", q[i]);
    }
    printf("\n");
    free(q);

    // printf("Realloc\n");
    char *r = malloc(2);
    for (int i = 0; i < 2; i++)
    {
        r[i] = i;
    }
    r = realloc(r, 20);
    for (int i = 0; i < 20; i++)
    {
        printf("%d ", r[i]);
    }
    // printf("\n");
    free(r);

    return 0;
}*/
