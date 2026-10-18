# mycd

Write `mycd [DIR]`, which changes the current directory and prints the **absolute path** of the new current directory.

- `DIR` can be absolute or relative.
- Without arguments it goes to the directory in the `HOME` environment variable.
- If `HOME` is needed but not set, print `mycd: HOME not set` to standard error and exit with status 1.
- If the directory cannot be entered, print `DIR: <reason>` (the text of `strerror(errno)`) to standard error and exit with status 1.
- With more than one argument print `usage: mycd [DIR]` to standard error and exit with status 2.

(A real `cd` is a shell built-in, because a child process cannot change its parent's directory. This program only changes **its own** directory, and shows it.)

**Examples**

```
$ ./prog /usr
/usr
$ HOME=/usr/bin ./prog
/usr/bin
```
