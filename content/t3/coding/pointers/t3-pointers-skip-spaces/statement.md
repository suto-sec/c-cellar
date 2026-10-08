# A pointer to a pointer

A hidden `main` reads a line and calls your function so that its pointer `p` skips the leading spaces. Then it prints the rest of the line between brackets. Write in `answer.c`:

```c
void skip_spaces(char **p);   /* advances *p past any ' ' characters */
```

To change where the **caller's** pointer points, you must receive its address (a pointer to a pointer).

**Example**

Input:
```
    indented text
```
Output:
```
[indented text]
```
