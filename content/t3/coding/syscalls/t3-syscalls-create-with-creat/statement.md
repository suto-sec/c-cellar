# Create with creat

`./prog FILE TEXT` creates `FILE` with the system call `creat` (emptying it if it already exists), with the permissions `rw-r--r--` (0644), and writes `TEXT` followed by a line break into it. Use only system calls for the file. If the arguments are not exactly two, or `creat` fails, exit with status 1.

**Example**

```
$ ./prog hello.txt "Hello there"
$ cat hello.txt
Hello there
```
