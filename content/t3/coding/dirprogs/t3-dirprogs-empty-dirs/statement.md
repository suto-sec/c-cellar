# Find the empty directories

Write `emptydirs DIR`, which prints the path of every **empty directory** found in `DIR`, at any depth, including `DIR` itself if it is empty. A directory is empty if it has no entries other than `.` and `..` (a hidden file makes it non-empty). Paths are written as `DIR/sub/sub2` (starting with `DIR` exactly as it was given). The order does not matter.

- `DIR` that cannot be opened: `emptydirs: DIR: <reason>` (`strerror(errno)`) to standard error and status 1. Wrong number of arguments: `usage: emptydirs DIR` and status 2.

**Example**

```
$ ./prog proj
proj/build
proj/src/empty
```
