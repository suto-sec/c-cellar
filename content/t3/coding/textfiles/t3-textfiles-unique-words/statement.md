# Unique words, sorted

Write `uniquewords FILE`, which prints every different word of the file once, in alphabetical order (`strcmp` order: capital letters come before lowercase), one per line.

- Words are separated by any white space and have at most 63 characters. Upper and lower case are different words.
- The file can contain any number of words.
- A file that cannot be opened: `uniquewords: FILE: <reason>` to standard error and status 1. Wrong number of arguments: `usage: uniquewords FILE` and status 2.

**Example**

```
$ cat f.txt
the cat and the dog
and the bird
$ ./prog f.txt
and
bird
cat
dog
the
```
