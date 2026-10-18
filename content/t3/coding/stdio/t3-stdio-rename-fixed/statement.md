# Rename one file

Write a program that renames the file `old.txt` to `new.txt`.

- If it worked print `renamed`.
- If it did not (for example `old.txt` does not exist) print `cannot rename` and exit with status 1.

## Functions for this exercise

- `int rename(const char *old, const char *new)` (in `<stdio.h>`) changes the name of a file. It returns `0` on success and `-1` on failure.

```c
if (rename("a.txt", "b.txt") == 0)
    printf("done\n");
```
