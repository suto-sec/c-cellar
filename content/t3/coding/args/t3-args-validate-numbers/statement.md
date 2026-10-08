# Validate numbers

For every argument print `ok <value>` if the **whole** argument is a valid integer (an optional sign and digits), or `invalid <argument>` otherwise. An empty argument is invalid. Use `strtol` and its end pointer (not `atoi`).

**Example**

```
$ ./prog 42 4x2 -1
ok 42
invalid 4x2
ok -1
```
