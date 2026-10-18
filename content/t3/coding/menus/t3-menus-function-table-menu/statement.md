# A table of functions

The input is a list of lines `OPTION A B`: an option and two numbers (they can have decimals). For every line print the result with two decimals:

| option | operation |
|---|---|
| `1` | `A + B` |
| `2` | `A - B` |
| `3` | `A * B` |
| `4` | `A / B`, or `division by zero` (and nothing else for that line) if `B` is 0 |
| any other | `invalid option` (the two numbers are still read and ignored) |

Option `0` stops the program, and so does the end of the input. A line with option `0` may have no numbers after it.

Implement the four operations as functions with the same signature and **call them through an array of pointers to functions**, indexed by the option, not through a `switch` that repeats the four calls.

**Example**

```
1 2 3
4 7 2
4 1 0
0
```
prints
```
5.00
3.50
division by zero
```
