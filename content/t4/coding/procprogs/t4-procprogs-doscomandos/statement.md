# doscomandos: ls | tr

Without calling the shell, do what `ls | tr a-z A-Z` does: the program creates **two child processes** and a pipe. The first child runs `ls`, with its standard output redirected into the pipe. The second runs `tr a-z A-Z`, with its standard input coming from the pipe. The parent waits for both.

The commands are written directly in the program.

**Example**

```
$ ls
a.txt  b.txt
$ ./prog
A.TXT
B.TXT
```
