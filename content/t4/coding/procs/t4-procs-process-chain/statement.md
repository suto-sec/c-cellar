# A chain of processes

`prog N` (with `N` of at least 1) builds a **chain** of `N` processes. The process at level `k` (the first one is level 1):

1. prints `down k`;
2. if `k < N`, creates the next level as its child and waits for it;
3. prints `up k`.

So the levels print `down` from outermost to innermost, and `up` from the innermost back out.

**Example**

```
$ ./prog 3
down 1
down 2
down 3
up 3
up 2
up 1
```
