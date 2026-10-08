# Create a directory

`makedir NAME` creates the directory `NAME` with permissions `0755` and prints `created NAME`. If it fails print `cannot create NAME: <reason>` (`strerror(errno)`) on standard error and exit with 1.

**Example**

```
$ ./prog photos && ls -d photos
created photos
photos
```
