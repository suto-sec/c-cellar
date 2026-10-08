# A tiny grep

`grepl [-n] PATTERN FILE` prints the lines of `FILE` that **contain** `PATTERN` (a plain substring, case-sensitive). With `-n` each printed line is preceded by its line number and a colon (`2:text`). Exit with status 0 if at least one line matched and 1 if none did; a missing file prints `cannot open FILE` on standard error and exits with 2. Lines have at most 1000 characters.

**Examples**

```
$ ./prog sat f.txt
The cat sat.
A dog sat on the mat.
```

```
$ ./prog -n sat f.txt
1:The cat sat.
2:A dog sat on the mat.
```
