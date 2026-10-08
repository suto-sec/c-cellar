# Trim a string in place

A hidden `main` reads a line, calls your function and prints the result between brackets. Write in `answer.c`:

```c
void trim(char *s);   /* removes the spaces at the start and at the end of s, in place */
```

Only the space character `' '` counts. `"  hi  "` becomes `"hi"`; a string of only spaces becomes empty.

**Example**

Input:
```
  hello world  
```
Output:
```
[hello world]
```
