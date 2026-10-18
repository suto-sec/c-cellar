# mywc

Write `mywc [-l|-w|-c]`, which reads **standard input** and counts:

- **lines**: the number of `'\n'` characters;
- **words**: maximal sequences of characters that are not white space (`isspace`);
- **bytes**: every character read.

Without options it prints the three numbers separated by single spaces: `LINES WORDS BYTES`. With `-l`, `-w` or `-c` it prints only that number.

Any other argument prints `usage: mywc [-l|-w|-c]` to standard error and exits with status 2.

**Example**

```
$ printf 'one two\nthree\n' | ./prog
2 3 14
$ printf 'one two\nthree\n' | ./prog -w
3
```
