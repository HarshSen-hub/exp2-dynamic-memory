/*
 * Experiment 2: Dynamic Memory Allocation, Reallocation, Deallocation
 * CORRECTED VERSION
 *
 * Fixes applied:
 *   1. Every malloc()/realloc() return value is checked before use.
 *   2. Memory is initialised immediately after (re)allocation.
 *   3. The pointer is set to NULL immediately after free(), so any
 *      accidental future use is an immediate, easily diagnosable
 *      NULL-pointer access instead of silent heap corruption.
 *
 * Compile with ASan (still useful to prove NO errors are reported):
 *   gcc -fsanitize=address -g -O0 -o fixed fixed.c
 */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n = 5;

    /* ---- Stage 1: malloc ---- */
    int *arr = (int *) malloc(n * sizeof(int));
    if (arr == NULL) {                       /* FIX 1: check return value */
        fprintf(stderr, "Error: malloc failed to allocate %zu bytes\n",
                n * sizeof(int));
        return EXIT_FAILURE;
    }
    for (int i = 0; i < n; i++) arr[i] = 0;  /* FIX 2: initialise */

    printf("Stage 1 (malloc):\n");
    printf("  Pointer value   : %p\n", (void *)arr);
    printf("  Allocated size  : %zu bytes (%d ints)\n", n * sizeof(int), n);

    for (int i = 0; i < n; i++) arr[i] = i + 1;
    printf("  Initialised values: ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n\n");

    /* ---- Stage 2: realloc ---- */
    int new_n = 10;
    int *tmp = (int *) realloc(arr, new_n * sizeof(int));
    if (tmp == NULL) {                       /* FIX 1: check return value */
        fprintf(stderr, "Error: realloc failed to grow to %zu bytes\n",
                new_n * sizeof(int));
        free(arr);                           /* original block still valid */
        arr = NULL;
        return EXIT_FAILURE;
    }
    arr = tmp;
    for (int i = n; i < new_n; i++) arr[i] = 0;  /* FIX 2: initialise new region */

    printf("Stage 2 (realloc):\n");
    printf("  Pointer value   : %p\n", (void *)arr);
    printf("  Allocated size  : %zu bytes (%d ints)\n", new_n * sizeof(int), new_n);

    for (int i = n; i < new_n; i++) arr[i] = (i + 1) * 10;
    printf("  Values after realloc: ");
    for (int i = 0; i < new_n; i++) printf("%d ", arr[i]);
    printf("\n\n");

    /* ---- Stage 3: free ---- */
    free(arr);
    printf("Stage 3 (free):\n");
    printf("  Pointer value freed : %p\n", (void *)arr);

    arr = NULL;                              /* FIX 3: avoid dangling pointer */
    printf("  Pointer value after setting to NULL: %p\n", (void *)arr);
    printf("  Memory safely returned to heap allocator.\n\n");

    /* Any accidental future use is now caught immediately: */
    if (arr != NULL) {
        arr[0] = 999;   /* unreachable, but shows the guard in place */
    } else {
        printf("Pointer is NULL - use-after-free is now impossible via this variable.\n");
    }

    return EXIT_SUCCESS;
}
