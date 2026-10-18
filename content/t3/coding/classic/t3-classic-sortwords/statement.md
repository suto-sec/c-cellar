# Sort the words of a file

Write `sortwords FILE.txt N`. The first argument is the name of a `.txt` file and the second is the **number of words** the file contains. The program loads the words into an array, sorts them alphabetically with a sorting method you write yourself (for example the bubble sort) and prints them one per line.

- Words are separated by spaces or newlines and have at most 31 characters. The file really contains `N` words.
- With a wrong number of arguments print `usage: sortwords FILE.txt N` to standard error and exit with status 2.
- If the name does not end in `.txt`, print `sortwords: FILE: not a .txt file` to standard error and exit with status 2.
- If `N` is not a positive integer print the usage message and exit with status 2.
- If the file does not exist (or cannot be opened) print `sortwords: FILE: <reason>` (`strerror(errno)`) to standard error and exit with status 1.
- Sort in the order of `strcmp`.

**Example**

```
$ cat f.txt
pear apple
fig
$ ./prog f.txt 3
apple
fig
pear
```
