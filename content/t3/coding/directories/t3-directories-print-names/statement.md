# Read the entries of a directory

Write `names DIR`, which prints the name of **every entry** of the directory, one per line. The order is whatever the system gives you. Do **not** skip anything: the entries `.` (the directory itself) and `..` (its parent) are real entries and are printed too.

- If `DIR` cannot be opened print `cannot open DIR` to standard error and exit with status 1. A wrong number of arguments: status 2.

## Functions for this exercise

You know `opendir` and `closedir`. The new one:

- `struct dirent *readdir(DIR *d)` returns the next entry of the directory, or `NULL` when there are no more. The name is in the field `d_name` of the returned structure: `e->d_name`.

```c
struct dirent *e;
while ((e = readdir(d)) != NULL)
    printf("%s\n", e->d_name);
```
