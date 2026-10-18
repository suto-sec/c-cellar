# Words grouped by length

Write `bylength FILE`. For every word length that appears in the file, in **ascending** order, print a line with the length, a colon and the words of that length in the order they appear in the file (repeated words are repeated), separated by single spaces. Lengths with no words print nothing.

- Words are separated by white space and have at most 100 characters. The file can have any number of words.
- A file with no words prints nothing.
- A file that cannot be opened: `bylength: FILE: <reason>` to standard error and status 1. Wrong number of arguments: `usage: bylength FILE` and status 2.

**Example**

```
$ cat f.txt
the quick brown fox jumps over the lazy dog
$ ./prog f.txt
3: the fox the dog
4: over lazy
5: quick brown jumps
```
