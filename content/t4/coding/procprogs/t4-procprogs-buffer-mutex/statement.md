# buffer-mutex: reader and writer

Two threads share a global buffer of **10000** integers.

- The **writer** does **200 passes**. In pass `p` (from 1) it writes the value `p` into **every** element of the buffer.
- The **reader** also does **200 passes**. In each one it checks that **all the elements are equal** (all the same as the first one); if not, that pass is *inconsistent*.

`prog [mutex]`: if the program gets the argument `mutex`, each full pass (a fill, or a check) runs under a **mutex**; without the argument the threads access the buffer without any protection. The main thread joins them and prints `all reads consistent` if no pass was inconsistent, or `inconsistent reads: N` otherwise.

With the mutex, the answer is always `all reads consistent`. (Without it, it usually shows inconsistent reads: that is the point.)

**Example**

```
$ ./prog mutex
all reads consistent
```
