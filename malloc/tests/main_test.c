#include <stdio.h>
#include <string.h>

#include "../malloc.h"

int main()
{
    printf("Malloc\n");
    char *p = malloc(20);
    if (!p)
        return 1;
    for (int i = 0; i < 20; i++)
        p[i] = i;
    for (int i = 0; i < 20; i++)
        printf("%d ", p[i]);
    printf("\n");
    free(p);

    printf("Calloc\n");
    char *q = calloc(5, 4); // 20 bytes
    for (int i = 0; i < 20; i++)
        printf("%d ", q[i]);
    printf("\n");
    free(q);

    printf("Realloc\n");
    char *r = malloc(10);
    for (int i = 0; i < 10; i++)
        r[i] = i;
    r = realloc(r, 20);
    for (int i = 0; i < 20; i++)
        printf("%d ", r[i]);
    printf("\n");
    free(r);

    return 0;
}
