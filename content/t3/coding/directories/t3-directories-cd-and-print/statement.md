# Change directory and print

`cdprint DIR` changes the current directory to `DIR` (absolute or relative) and prints the absolute path of the new directory. If it fails, print `DIR: <reason>` (use `strerror(errno)`) on standard error and exit with 1.

**Example**

```
$ ./prog /usr
/usr
```
