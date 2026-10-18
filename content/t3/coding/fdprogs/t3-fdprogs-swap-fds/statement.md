# Swap stdout and stderr

Write `swapfds`, which exchanges what descriptors **1 and 2 refer to** (using `dup` and `dup2`, and closing any helper descriptor) and then writes, with `write(2)`:

- the text `to descriptor 1` and a newline on descriptor 1;
- the text `to descriptor 2` and a newline on descriptor 2.

Because the descriptors were swapped, the first text appears where the original standard error was, and the second where the original standard output was. It prints nothing else and takes no arguments (`usage: swapfds` on standard error and status 2 otherwise).

**Example**

```
$ ./prog 2>/dev/null
to descriptor 2
$ ./prog >/dev/null
to descriptor 1
```
