# Count the capital letters

Write a program that reads **one line** from standard input (at most 999 characters) and prints `capitals: N`, with `N` the number of upper case letters (`A`-`Z`) in it. Everything else (lower case letters, digits, spaces, punctuation) is not counted. An empty input gives `capitals: 0`.

## Functions for this exercise

- `int isupper(int c)` (in `<ctype.h>`) returns non-zero if `c` is an upper case letter and `0` otherwise. Its twin `islower` does the same for lower case.

```c
#include <ctype.h>

if (isupper(line[i]))
    count++;
```
