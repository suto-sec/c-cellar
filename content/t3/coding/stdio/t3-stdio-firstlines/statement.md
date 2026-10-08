# firstlines

Write `firstlines [-N]`: copy the first `N` lines of standard input to standard output (`N` defaults to 5).
Use `fgets` with a buffer of 1024 bytes.

```
$ seq 1 20 | ./prog -3
1
2
3
```

- With fewer than `N` lines, copy all of them.
- Output must be byte-for-byte what was read: if the last line has no final newline, do not add one.
- **Lines can be longer than your buffer.** A 3000-character line is still *one* line.
