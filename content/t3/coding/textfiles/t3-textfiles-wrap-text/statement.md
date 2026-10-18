# Wrap text to a width

Write `wrap WIDTH`, which reads **standard input** and prints its words again, filling each output line with as many words as fit in `WIDTH` characters:

- The words are all the white-space separated words of the input, in order. Line breaks and blank lines in the input do not matter: it is all one text.
- Words in an output line are separated by **one space**. A line never has more than `WIDTH` characters, unless it is a single word longer than `WIDTH`, which goes alone on its line.
- No spaces at the end of a line. If the input has no words print nothing; otherwise the last line ends with `\n`.
- Words have at most 200 characters. `WIDTH` must be a positive integer, otherwise (or with the wrong number of arguments) print `usage: wrap WIDTH` to standard error and exit with status 2.

**Example**

```
$ echo 'the quick brown fox jumps over the lazy dog' | ./prog 15
the quick brown
fox jumps over
the lazy dog
```
