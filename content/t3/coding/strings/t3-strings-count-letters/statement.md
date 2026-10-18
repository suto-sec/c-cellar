# Count the letters

Write a program that reads **one line** from standard input (at most 999 characters) and prints `letters: N`, with `N` the number of letters (`a`-`z` and `A`-`Z`) in it. Digits, spaces and punctuation are not letters. An empty input gives `letters: 0`.

## Functions for this exercise

- `int isalpha(int c)` (in `<ctype.h>`) returns non-zero if the character `c` is a letter and `0` otherwise. There are similar ones for other classes (`isdigit`, `isspace`...).
- `fgets(buf, size, stdin)` reads a line into `buf` (with the `'\n'` included) and returns `NULL` at the end of the input.

```c
#include <ctype.h>

if (isalpha(line[i]))
    count++;
```
