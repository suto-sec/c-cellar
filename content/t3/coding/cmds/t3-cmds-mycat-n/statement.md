# mycat with -n

Write `mycat [-n] [FILE...]`, which writes the files one after the other to standard output.

- Without files (or for a file called `-`) it reads **standard input**.
- With `-n` (always the first argument) every output line is preceded by its number, **continuing across files**, in the format `"%6d\t"` (as `cat -n` does). Empty lines are numbered too. A final line without `\n` is numbered and printed as it is.
- A file that cannot be opened prints `mycat: FILE: <reason>` (`strerror(errno)`) to standard error. The other files are still processed, and the exit status at the end is 1 (0 if everything went well).

**Example**

```
$ printf 'a\nb\n' > x.txt
$ ./prog -n x.txt x.txt
     1	a
     2	b
     3	a
     4	b
```
