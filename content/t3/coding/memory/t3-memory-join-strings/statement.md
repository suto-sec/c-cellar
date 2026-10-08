# Join two strings

A hidden `main` reads two words per line, joins them with your function and prints the result. Write in `answer.c`:

```c
char *join(const char *a, const char *b, char sep);   /* a, then sep, then b, in a malloc'ed string */
```

**Example**

Input:
```
north south
```
Output:
```
north-south
```
