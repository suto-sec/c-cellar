# Sum what arrives until the pipe closes

Read integers from standard input (as many as there are, until end of file). The **parent** sends each one down a pipe to a **child**; the child adds them up and, when the pipe is closed, prints `sum: S`. The parent waits for the child.

The child does not know how many numbers will come: it must stop when `read` returns 0.

**Examples**

```
$ echo 1 2 3 | ./prog
sum: 6
$ ./prog < /dev/null
sum: 0
```
