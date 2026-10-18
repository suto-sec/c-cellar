# Split a file in pieces

Write `splitfile FILE N`, which copies the lines of `FILE` into new files of `N` lines each, named `FILE.part1`, `FILE.part2`, ... (the name of `FILE` exactly as it was given, followed by `.partK`). The last piece can have fewer lines. An empty `FILE` creates no pieces. `FILE` itself is not modified.

For every piece created print `NAME: K lines` (the name of the piece and its number of lines, always the word `lines`).

- Lines have at most 1000 characters. The pieces are copies of the lines, byte for byte.
- `N` must be a positive integer; otherwise (or with the wrong number of arguments) print `usage: splitfile FILE N` to standard error and exit with status 2.
- A `FILE` that cannot be opened: `splitfile: FILE: <reason>` to standard error and status 1. A piece that cannot be created: `splitfile: NAME: <reason>` and status 1.

**Example**

```
$ seq 1 7 > f.txt
$ ./prog f.txt 3
f.txt.part1: 3 lines
f.txt.part2: 3 lines
f.txt.part3: 1 lines
$ cat f.txt.part3
7
```
