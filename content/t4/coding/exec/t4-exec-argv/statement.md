# Run the command you are given

`prog CMD [ARGS...]` creates a child that runs `CMD` with its arguments (use `execvp`, which looks for `CMD` in the PATH). The parent waits and then prints `finished`.

If the command cannot be run, the child ends with status `127` and prints nothing; the parent still prints `finished`.

**Examples**

```
$ ./prog echo a b
a b
finished
$ ./prog nosuchcommand
finished
```
