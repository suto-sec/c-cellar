# Sum the numbers of a file

Write `sumfile FILE`. The file contains integers separated by spaces or newlines. Print their sum (just the number). An empty file gives `0`.

- A file that cannot be opened: print `cannot open FILE` to standard error and exit with status 1. A wrong number of arguments: status 2.

## Functions for this exercise

- `int fscanf(FILE *f, const char *format, ...)` is `scanf` reading from the file `f`. As with `scanf` you pass the **address** of the variable (`&x`) and it returns how many values it read: `1` for one `%d`, and something else (`0` or `EOF`) when there are no more numbers.

```c
int x;
while (fscanf(f, "%d", &x) == 1) {
    /* use x */
}
```
