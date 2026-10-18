# Palindrome words

Write `palwords FILE`, which prints, in the order they appear, the words of the file that are **palindromes**: they read the same forwards and backwards (`level`, `abba`). Words are separated by white space (up to 100 characters) and the comparison is exact (`Level` is not a palindrome). A word of one character is not counted.

After the words print `total: N`, with the number of palindromes printed (also when it is 0).

- A file that cannot be opened: `palwords: FILE: <reason>` to standard error and status 1. Wrong number of arguments: `usage: palwords FILE` and status 2.

**Example**

```
$ cat f.txt
a level of noon is rare
$ ./prog f.txt
level
noon
total: 2
```
