# Count the signals

`prog N` installs a handler for `SIGUSR1` that counts how many times it runs, sends the signal to itself `N` times with `raise` and finally prints `received N`.

**Examples**

```
$ ./prog 5
received 5
$ ./prog 0
received 0
```
