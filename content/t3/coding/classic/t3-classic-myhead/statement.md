# myhead

Write `myhead`, which behaves like `head(1)` reading **standard input**: it prints the first 10 lines it receives, or the first `N` lines when called as `myhead -N`.

- Read with `fgets(buf, 1024, stdin)` (lines have fewer than 1000 characters).
- With fewer than `N` lines, print all of them. `myhead -0` prints nothing.
- Print the lines exactly as they come: if the last line has no `\n`, do not add one.
- Any other form of call prints `usage: myhead [-N]` to standard error and exits with status 2.

**Example**

```
$ seq 1 100 | ./prog -3
1
2
3
```
