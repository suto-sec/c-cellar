# Which descriptor?

The program prints `stdout = <descriptor of stdout>`, then opens `out.txt` for writing with `fopen` and prints `file = <descriptor of that FILE>` (both numbers obtained with `fileno`). Finally it writes the line `hello` into the file using the **system call** `write` on that descriptor, and closes the file.

**Example**

```
$ ./prog
stdout = 1
file = 3
$ cat out.txt
hello
```
