# Many writers, one reader

`prog N` creates one pipe and `N` children (from 1 to 8). Child number `i` (from 1) writes the line `worker i` (with its newline, in a single `write`) into the pipe and ends.

The parent reads everything from the pipe until the end and copies it to standard output, then waits for all the children. The lines may come out in any order.

**Example**

```
$ ./prog 3
worker 2
worker 1
worker 3
```
