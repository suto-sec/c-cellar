# Leave no descriptors behind

Repeat **200 times**:

1. create a pipe;
2. create a child that sends its standard output into the pipe and runs `echo hi` (`execlp`);
3. in the parent, read from the pipe until the end of the data, adding up the bytes you read;
4. close what you opened and collect the child with `waitpid`.

After the loop print the total of bytes read, then the number that `dup(0)` returns (the lowest free descriptor, `3` when you have not leaked any).

**Example**

```
$ ./prog
bytes: 600
next free descriptor: 3
```

(Each `echo hi` writes 3 bytes: `h`, `i` and the newline. Leaking even one descriptor per round would make the last number bigger, and after about a thousand `pipe` would start failing.)
