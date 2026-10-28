# Catch a signal

Install a handler for `SIGUSR1`. The handler prints `got SIGUSR1`. The program then sends that signal **to itself** with `raise(SIGUSR1)` and, once `raise` returns, prints `after`.

**Example**

```
$ ./prog
got SIGUSR1
after
```
