# A directory change that does not last

`prog DIR` creates a child that tries to go to `DIR` with `chdir`:

- if it works, the child prints `child: NAME`, where `NAME` is the last component of its new current directory (as `getcwd` reports it);
- if it fails, the child prints `child: cannot go to DIR: REASON` (`REASON` is the text of the error) and ends.

The parent waits and then prints `parent: unchanged` if its own current directory is still the one it had at the start, or `parent: changed` otherwise.

**Example**

```
$ mkdir sub
$ ./prog sub
child: sub
parent: unchanged
$ ./prog nosuch
child: cannot go to nosuch: No such file or directory
parent: unchanged
```
