# The v variants take an array

Run `echo one two three` in a child with **`execv`** (not `execlp`): build an array of strings ending in `NULL` and pass it with the full path `/bin/echo`. The parent waits and then prints `done`.

**Example**

```
$ ./prog
one two three
done
```
