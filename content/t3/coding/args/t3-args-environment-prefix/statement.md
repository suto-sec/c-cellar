# Variables with a prefix

`./prog PREFIX` prints every environment variable whose name starts with `PREFIX`, as `NAME=value`, one per line, in any order. Use the third parameter of `main` (`envp`) instead of `getenv`. If the number of arguments is not 1, print nothing and exit with status 1.

**Example**

```
$ APP_MODE=debug APP_LEVEL=3 OTHER=x ./prog APP_
APP_MODE=debug
APP_LEVEL=3
```
