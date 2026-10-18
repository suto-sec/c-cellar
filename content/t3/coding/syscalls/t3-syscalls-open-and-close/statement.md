# Open a file with open

Write `opener FILE`, which opens the file for reading with the system call `open`:

- If it worked print `opened with descriptor N` (where `N` is the number `open` returned), close it and exit with status 0.
- If it failed print `cannot open FILE` and exit with status 1.

A wrong number of arguments: status 2.

## Functions for this exercise

- `int open(const char *path, int flags)` (in `<fcntl.h>`) opens a file. The flag `O_RDONLY` means "only to read". It returns a **descriptor**, a small number that identifies the open file in the next calls, or `-1` if it fails.
- `int close(int fd)` (in `<unistd.h>`) closes a descriptor.

```c
#include <fcntl.h>
#include <unistd.h>

int fd = open("data.txt", O_RDONLY);
if (fd == -1)
    return 1;
close(fd);
```
