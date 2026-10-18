# Capitalize every word

Write `capitalize FILE`, which copies the file to standard output changing each **word** so that its first letter is a capital and the other letters are lower case. A word is a maximal sequence of letters (`isalpha`), so in `don't` there are two words (`Don'T`) and in `a1b` too (`A1B`). Every other character, including newlines and a missing final newline, is copied exactly.

- A file that cannot be opened: `capitalize: FILE: <reason>` to standard error and status 1. Wrong number of arguments: `usage: capitalize FILE` and status 2.

**Example**

```
$ cat f.txt
hELLO wORLD, it's me
$ ./prog f.txt
Hello World, It'S Me
```
