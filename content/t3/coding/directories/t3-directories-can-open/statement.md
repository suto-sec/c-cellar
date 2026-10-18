# Open a directory

Write `diropen PATH`, which tries to open `PATH` as a directory:

- If it works print `PATH is a directory` (and close it).
- If not print `cannot open PATH` and exit with status 1.

A wrong number of arguments: status 2.

## Functions for this exercise

- `DIR *opendir(const char *path)` (in `<dirent.h>`) opens a directory. It returns a `DIR *` (a handle, like the `FILE *` of `fopen`) or `NULL` if it fails: the path does not exist or is not a directory.
- `int closedir(DIR *d)` closes it.

```c
#include <dirent.h>

DIR *d = opendir(".");
if (d != NULL)
    closedir(d);
```
