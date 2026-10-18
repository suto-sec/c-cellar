# Copy a string

Write `copyword WORD`, which copies `WORD` into an array of its own, replaces the **first character of the copy** by `*`, and prints two lines:

```
original: WORD
copy: *ORD
```

`WORD` has at most 90 characters. A wrong number of arguments: status 2.

## Functions for this exercise

- `char *strcpy(char *dest, const char *src)` (in `<string.h>`) copies the string `src`, with its final `'\0'`, into `dest`. `dest` must be an array **big enough** for the text.
- A string is an array of characters: `copy[0]` is its first character and you can assign to it: `copy[0] = '*';`

```c
char copy[100];
strcpy(copy, "hello");
copy[1] = 'a';            /* now it is "hallo" */
```
