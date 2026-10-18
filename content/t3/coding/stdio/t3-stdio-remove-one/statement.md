# Delete one file

Write a program that deletes the file `junk.txt`.

- If it worked print `removed`.
- If it did not (for example the file does not exist) print `nothing to remove` and exit with status 1.

## Functions for this exercise

- `int remove(const char *name)` (in `<stdio.h>`) deletes a file. It returns `0` on success and `-1` on failure.

```c
if (remove("a.txt") == 0)
    printf("deleted\n");
```
