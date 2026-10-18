# Read a setting

Write `getkey FILE KEY`. `FILE` is a configuration file with lines `KEY=VALUE`. Print the **value** of `KEY`, taken from the **first** line that defines it.

- The key must match **exactly** (`host` does not match `hostname=...`) and is followed immediately by `=`. There are no spaces around `=` to remove: `name = x` defines the key `name ` (with a space).
- The value is the rest of the line after the first `=`; it can be empty or contain more `=` characters.
- Lines that start with `#` and empty lines are ignored.
- Key not found: print `getkey: KEY: not found` to standard error and exit with status 1.
- File that cannot be opened: `getkey: FILE: <reason>` (`strerror(errno)`) to standard error and exit with status 2. Wrong number of arguments: `usage: getkey FILE KEY` and status 2.

**Example**

```
$ cat conf.txt
host=localhost
port=8080
$ ./prog conf.txt port
8080
```
