# Numbers in a file

Write a program that creates `numbers.txt` containing the integers from 1 to 5, one per line. It prints nothing on the screen.

## Functions for this exercise

You already know `fopen`, and `fclose` from the previous exercise. The new one is `fprintf`:

- `fprintf(f, format, ...)` is `printf` that writes into the file `f` instead of the screen. The first argument is the file; everything else is as in `printf`.

```c
FILE *f = fopen("name.txt", "w");
fprintf(f, "%d squared is %d\n", 4, 16);
fclose(f);
```

Use a `for` loop: open the file once before it and close it once after it.
