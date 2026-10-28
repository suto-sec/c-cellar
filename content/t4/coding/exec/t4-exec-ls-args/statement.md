# List a directory in a child

`prog DIR` creates a child that runs `ls -1 DIR` with `execlp`. The parent waits for it and then prints `listing finished`.

**Example**

```
$ ./prog docs
notes.txt
todo.txt
listing finished
```
