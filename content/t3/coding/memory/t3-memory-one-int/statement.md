# Your first malloc

Write a program that reads an integer from standard input, stores **twice its value** in memory obtained with `malloc` (space for a single `int`), prints `double: N` using that memory, and frees it. If the input is not a number print `bad input` and exit with status 1. If `malloc` fails exit with status 1.

## Functions for this exercise

- `void *malloc(size_t bytes)` (in `<stdlib.h>`) reserves that many bytes and returns a pointer to them, or `NULL` if there is no memory. `sizeof(int)` is the number of bytes of one `int`.
- `void free(void *p)` gives the memory back. Call it once for every `malloc`, when you no longer need it.

```c
#include <stdlib.h>

int *p = malloc(sizeof(int));
if (p == NULL)
    return 1;
*p = 5;                    /* write through the pointer */
printf("%d\n", *p);        /* read through the pointer */
free(p);
```
