# Your own strcpy

A hidden `main` reads words, calls your function to copy each into a buffer and prints the copy. Write in `answer.c`, **without** `strcpy`:

```c
void my_strcpy(char *dst, const char *src);   /* copies src, including its '\0', into dst */
```

**Example**

Input:
```
hello
```
Output:
```
copy: hello
```
