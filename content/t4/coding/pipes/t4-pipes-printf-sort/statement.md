# Two commands joined by a pipe

Without using the shell, make the program do what `printf 'pear\napple\nfig\n' | sort` does: create **two** children and a pipe. The first runs `printf` with the text `pear\napple\nfig\n`; its output goes into the pipe. The second runs `sort`, reading from the pipe. The parent waits for both.

**Example**

```
$ ./prog
apple
fig
pear
```
