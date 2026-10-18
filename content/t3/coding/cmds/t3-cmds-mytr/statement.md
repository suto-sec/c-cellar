# mytr

Write `mytr`, a small `tr(1)`, which copies **standard input** to standard output changing characters:

```
mytr FROM TO      each character of FROM becomes the character at the same position in TO
mytr -d SET       the characters that are in SET are deleted
```

- `FROM` and `TO` must have the same length, and the characters of `FROM` are all different. (There are no ranges: `a-z` means the three characters `a`, `-`, `z`.)
- If the lengths differ print `mytr: FROM and TO must have the same length` to standard error and exit with status 2.
- Any other way of calling it prints `usage: mytr FROM TO | mytr -d SET` to standard error and exits with status 2.

**Examples**

```
$ echo hello world | ./prog lo 01
he001 w1r0d
$ echo hello world | ./prog -d lo
he wrd
```
