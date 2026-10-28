# Print once, not twice

The parent prints `start ` (with a space, **no** newline), then creates a child. The child prints `child` and a newline. The parent waits, then prints `parent` and a newline.

The word `start` must appear **once**:

```
$ ./prog | cat
start child
parent
```

(The checker reads your output through a pipe, like `| cat` here. On a terminal the bug would stay hidden because the terminal flushes at every newline.)
