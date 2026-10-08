# Parse a number safely

`safenum TEXT` converts `TEXT` to a `long` with `strtol`. Print the value if the whole text is a valid number that fits in a `long`. Otherwise print `error: not a number` (nothing converted, or extra characters) or `error: out of range` (overflow) on standard output **and exit with status 1**. Success exits with 0.

**Examples**

```
$ ./prog 123
123
```

```
$ ./prog 12abc
error: not a number
```
