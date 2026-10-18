# Longest and shortest word

Write `extremes FILE`, which prints the longest and the shortest word of the file:

```
longest: <word>
shortest: <word>
```

- Words are separated by white space and have at most 1000 characters. When several words have the same length the **first one** in the file is the answer.
- A file with no words prints `extremes: no words` to standard error and exits with status 1.
- A file that cannot be opened: `extremes: FILE: <reason>` to standard error and status 2. Wrong number of arguments: `usage: extremes FILE` and status 2.

**Example**

```
$ cat f.txt
a tiny sentence with words
$ ./prog f.txt
longest: sentence
shortest: a
```
