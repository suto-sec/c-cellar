# The child sends a number

The child computes `6 * 7` and writes the resulting `int` into a pipe (the four bytes of the integer, not text). The parent reads the integer from the pipe and prints `answer: 42`.

**Example**

```
$ ./prog
answer: 42
```
