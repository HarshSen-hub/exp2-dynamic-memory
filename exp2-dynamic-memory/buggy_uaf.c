/*
 * Experiment 2 - Bug (b): Use-after-free via a dangling pointer
 *
 * Compile with ASan:
 *   gcc -fsanitize=address -g -O0 -o buggy_uaf buggy_uaf.c
 */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n = 5;

    /* ---- Stage 1: malloc ---- */
    int *arr = (int *) malloc(n * sizeof(int));
    if (arr == NULL) {
        fprintf(stderr, "malloc failed\n");
        return 1;
    }
    printf("Stage 1 (malloc):\n");
    printf("  Pointer value  : %p\n", (void *)arr);
    printf("  Allocated size : %zu bytes (%d ints)\n\n", n * sizeof(int), n);
    fflush(stdout);

    for (int i = 0; i < n; i++) arr[i] = i + 1;

    /* ---- Stage 2: realloc ---- */
    int new_n = 10;
    int *tmp = (int *) realloc(arr, new_n * sizeof(int));
    if (tmp == NULL) {
        fprintf(stderr, "realloc failed\n");
        free(arr);
        return 1;
    }
    arr = tmp;
    printf("Stage 2 (realloc):\n");
    printf("  Pointer value  : %p\n", (void *)arr);
    printf("  Allocated size : %zu bytes (%d ints)\n\n", new_n * sizeof(int), new_n);
    fflush(stdout);

    for (int i = n; i < new_n; i++) arr[i] = (i + 1) * 10;

    /* ---- Stage 3: free ---- */
    free(arr);
    printf("Stage 3 (free):\n");
    printf("  Pointer value freed : %p\n", (void *)arr);
    printf("  Memory returned to heap allocator.\n\n");
    fflush(stdout);

    /* BUG: 'arr' is now a dangling pointer. We deliberately keep using it. */
    printf("Demonstrating use-after-free:\n");
    printf("  Reading arr[0] after free...\n");
    fflush(stdout);
    printf("  arr[0] = %d\n", arr[0]);   /* USE AFTER FREE (read) */

    printf("  Writing arr[0] after free...\n");
    arr[0] = 999;                        /* USE AFTER FREE (write) */

    return 0;
}
