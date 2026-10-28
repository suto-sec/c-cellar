# Count the bytes that cross

The parent copies everything it reads from standard input into a pipe (using `read` and `write`, in chunks). The child reads the pipe until it is closed and prints `bytes: N`, the total number of bytes received. The parent waits for the child.

**Examples**

```
$ echo hello | ./prog
bytes: 6
$ ./prog < /dev/null
bytes: 0
```
