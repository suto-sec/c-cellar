# A program that replaces itself

Write a program that:

1. prints `before`;
2. replaces itself, **without** `fork`, by the command `echo replaced` (with `execlp`);
3. has a line that would print `after`, which never runs.

**Example**

```
$ ./prog | cat
before
replaced
```

(Mind the buffers: the checker reads your output through a pipe, where `printf` waits for the end of the program to write.)
