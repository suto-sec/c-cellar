# myuniq

Write `myuniq [-c]`, which reads **standard input** and writes it collapsing each group of **adjacent** equal lines into a single line (non-adjacent repetitions are not merged).

- With `-c` each line is preceded by the number of times it appeared, in the format `"%7d "` (as `uniq -c` does).
- Lines have at most 1000 characters and always end in `\n`.
- Any other argument prints `usage: myuniq [-c]` to standard error and exits with status 2.

**Example**

```
$ printf 'a\na\nb\na\n' | ./prog -c
      2 a
      1 b
      1 a
```
