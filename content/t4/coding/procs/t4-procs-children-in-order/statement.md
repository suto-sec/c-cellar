# One child at a time

Read an integer `N` from standard input. Create `N` children **one after another**: child number `i` (from 1) prints `child i` and ends, and the parent waits for it before creating the next one. When all are done the parent prints `all done`.

**Examples**

```
$ echo 3 | ./prog
child 1
child 2
child 3
all done
$ echo 0 | ./prog
all done
```
