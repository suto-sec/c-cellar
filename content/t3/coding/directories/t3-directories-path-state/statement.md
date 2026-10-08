# What is this path?

`pathstate PATH` prints `directory` if `PATH` can be opened as a directory, `not a directory` if it exists but is not one (`errno` is `ENOTDIR`), `missing` if it does not exist (`ENOENT`), and `error` for any other failure.

**Example**

```
$ ./prog /usr
directory
```
