# Redirect standard output

`redirect FILE`: first print `before` on standard output (with `printf`). Then redirect the standard output to `FILE` (created or emptied, permissions 0644) with `open` and `dup2`, and print `inside the file` with `printf`. After the program ends, `before` must be on the terminal and `inside the file` in the file.

Beware: the text of the first `printf` must really be written **before** you change descriptor 1 (`fflush(stdout)`).

**Example**

```
$ ./prog out.txt; echo ---; cat out.txt
before
---
inside the file
```
