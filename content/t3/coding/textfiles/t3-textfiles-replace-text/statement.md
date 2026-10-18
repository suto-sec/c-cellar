# Replace text

Write `replace FILE OLD NEW`, which copies the file to standard output replacing **every occurrence** of the text `OLD` by `NEW` (searching from left to right, without overlaps: `aa` in `aaa` is replaced once). `NEW` can be longer, shorter or empty (then `OLD` is deleted). Lines have at most 1000 characters and a replaced line may be longer than that, so print piece by piece.

- If `OLD` is empty, or the number of arguments is wrong, print `usage: replace FILE OLD NEW` to standard error and exit with status 2.
- A file that cannot be opened: `replace: FILE: <reason>` to standard error and status 1.

**Example**

```
$ cat f.txt
the cat sat on the mat
$ ./prog f.txt at og
the cog sog on the mog
```
