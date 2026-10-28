# A tiny system()

Read one line from standard input and run it as a shell command: a child runs `/bin/sh -c LINE` (`execl`). The parent waits and prints `status N`, with the command's exit status.

**Examples**

```
$ echo 'echo hi' | ./prog
hi
status 0
$ echo 'echo a; exit 4' | ./prog
a
status 4
```
