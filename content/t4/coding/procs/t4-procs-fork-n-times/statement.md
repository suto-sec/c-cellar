# 2 to the N

`prog N` runs `for (i = 0; i < N; i++) fork();` and then every process, original and children, prints `x`.

So the output has `2^N` lines. Do not wait for the children, do not use `exit` in them: this is the classic exam question *how many processes does this create?*.

**Examples**

```
$ ./prog 2 | wc -l
4
$ ./prog 0
x
```
