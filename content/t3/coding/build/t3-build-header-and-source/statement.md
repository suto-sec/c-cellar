# A header and a source file

Split a program into three files. You write **`util.h`** and **`util.c`** (and `main.c`):

- `util.h` declares `int clamp(int v, int lo, int hi);` (the value `v` limited to the range `lo..hi`);
- `util.c` includes `util.h` and defines `clamp`;
- `main.c` includes `util.h`, reads three integers `v lo hi` and prints `clamp(v, lo, hi)`.

It is built with `gcc -Wall -Wextra -o prog main.c util.c`. Use an include guard in the header.

**Example**

Input:
```
15 0 10
```
Output:
```
10
```
