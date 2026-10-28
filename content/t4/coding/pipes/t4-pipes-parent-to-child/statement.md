# A message down the pipe

The parent creates a pipe and a child. The parent writes the text `hello from parent` into the pipe; the child reads it and prints `child read: hello from parent`. The parent waits for the child.

**Example**

```
$ ./prog
child read: hello from parent
```
