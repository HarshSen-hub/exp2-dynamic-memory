/*
 * Experiment 2 - Bug (a): Unchecked malloc() return value
 * -> NULL pointer dereference
 *
 * We deliberately request an absurdly large block (larger than any
 * real system can supply) so malloc() genuinely returns NULL, then
 * we use the pointer WITHOUT checking it, causing a NULL dereference.
 *
 * Compile with ASan:
 *   gcc -fsanitize=address -g -O0 -o buggy_null buggy_null.c
 */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    /* Ask for an impossible amount of memory to force malloc() to fail */
    size_t huge_n = (size_t)-1 / 2;   /* ~ SIZE_MAX/2 elements */
    int *arr = (int *) malloc(huge_n * sizeof(int));

    printf("Requested elements : %zu\n", huge_n);
    printf("Pointer value      : %p\n", (void *)arr);   /* will print (nil) */
    fflush(stdout);

    /* BUG: no NULL check here before using arr */
    printf("Writing to arr[0]...\n");
    fflush(stdout);
    arr[0] = 42;   /* NULL POINTER DEREFERENCE */

    printf("This line is never reached.\n");
    return 0;
}
