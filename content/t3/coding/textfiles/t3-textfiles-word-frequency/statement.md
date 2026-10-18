# How many times each word

Write `wordfreq FILE`, which prints every different word of the file with the number of times it appears, in the order of its **first appearance**, one per line as `word: count`.

- Words are separated by white space and have at most 63 characters. Upper and lower case are different words.
- The file can contain any number of different words.
- A file that cannot be opened: `wordfreq: FILE: <reason>` to standard error and status 1. Wrong number of arguments: `usage: wordfreq FILE` and status 2.

**Example**

```
$ cat f.txt
to be or not to be
$ ./prog f.txt
to: 2
be: 2
or: 1
not: 1
```
