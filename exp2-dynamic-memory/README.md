# Experiment 2 — Dynamic Memory Allocation, Reallocation & Deallocation

Demonstrates `malloc`/`realloc`/`free` in C, along with two classic
memory-safety bugs and their AddressSanitizer (ASan) diagnostics, plus
a corrected version.

## Files
- `buggy_null.c` — Bug (a): unchecked `malloc()` return value → NULL pointer dereference
- `buggy_uaf.c` — Bug (b): use-after-free via a dangling pointer
- `fixed.c` — Corrected version: checked return values, initialised memory, pointer nulled after `free()`
- `*_out.txt` — Captured ASan diagnostic output for each program

## Build & run (Linux, gcc)
```
gcc -fsanitize=address -g -O0 -o buggy_null buggy_null.c
ASAN_OPTIONS=allocator_may_return_null=1 ./buggy_null

gcc -fsanitize=address -g -O0 -o buggy_uaf buggy_uaf.c
./buggy_uaf

gcc -fsanitize=address -g -O0 -o fixed fixed.c
./fixed
```
