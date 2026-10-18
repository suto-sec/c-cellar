# mycut

Write `mycut -dC -fN`, which reads **standard input** and prints, for every line, its `N`-th field (starting at 1) when the line is split at the delimiter character `C`.

- Both options are written attached to their value (`-d,` and `-f2`) and can come in either order.
- **Empty fields count**: in `a,,c` field 2 is empty and field 3 is `c`.
- A line **without any delimiter** is printed whole. A line with the delimiter but fewer than `N` fields prints an empty line.
- Lines have at most 1000 characters. If an option is missing, is not a single character (`-d`), or `N` is not a positive number, print `usage: mycut -dC -fN` to standard error and exit with status 2.

**Example**

```
$ printf 'ann:30:Madrid\nbob::Leon\n' | ./prog -d: -f2
30

```
