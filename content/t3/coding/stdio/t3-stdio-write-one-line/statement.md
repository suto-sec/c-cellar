# Your first file

Write a program that creates a file called `hello.txt` in the current directory containing one line: `Hello, file!`

It prints nothing on the screen. If `hello.txt` already exists, its old content is replaced.

## Functions for this exercise

- `FILE *fopen(const char *name, const char *mode)` opens a file. With the mode `"w"` the file is created (or emptied if it exists). It returns a `FILE *`, a handle that you give to the next calls.
- `fputs(text, f)` writes `text` into the file `f`, like `puts` writes on the screen, but it does **not** add a newline: put the `\n` in the text.
- `fclose(f)` closes the file. Always do it when you finish writing, otherwise the text may never reach the file.

```c
FILE *f = fopen("name.txt", "w");
fputs("some text\n", f);
fclose(f);
```

**Check it** in the terminal: run `./prog` and then `cat hello.txt`.
