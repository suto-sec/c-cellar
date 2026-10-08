# Your own strdup

A hidden `main` reads words, duplicates each with your function, prints the copy and frees it. Write in `answer.c`, **without** `strdup`:

```c
char *my_strdup(const char *s);   /* a malloc'ed copy of s (the caller frees it); NULL if out of memory */
```

**Example**

Input:
```
memory
```
Output:
```
copy: memory
```
