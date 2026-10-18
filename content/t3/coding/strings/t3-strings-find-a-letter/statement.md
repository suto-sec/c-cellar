# Where is the letter?

Write `position TEXT LETTER`, which prints the position (counting from 0) of the **first** occurrence of the character `LETTER` (the first character of the second argument) in `TEXT`, or `not found` if `TEXT` does not contain it. A wrong number of arguments: exit with status 2.

## Functions for this exercise

- `char *strchr(const char *s, int c)` (in `<string.h>`) looks for the character `c` in the string `s`. It returns a **pointer** to the first one found, or `NULL`.
- Two pointers into the same string can be subtracted: the result is the distance, i.e. the index.

```c
char word[] = "banana";
char *p = strchr(word, 'n');
if (p != NULL)
    printf("%ld\n", (long) (p - word));   /* 2 */
```
