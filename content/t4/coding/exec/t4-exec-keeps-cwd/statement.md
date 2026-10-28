# exec keeps the directory

`prog DIR` changes the **parent's** current directory to `DIR` (`chdir`), then creates a child that runs the command `pwd`. The parent waits for it and prints nothing else.

If `chdir` fails, print `cannot enter DIR` to standard error and end with status 1.

**Example**

```
$ ./prog /usr/bin
/usr/bin
```
