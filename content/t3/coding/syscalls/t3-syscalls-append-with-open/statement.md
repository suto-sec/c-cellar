# Append with O_APPEND

`appendline FILE TEXT` appends the line `TEXT` to `FILE` with `open(..., O_WRONLY | O_CREAT | O_APPEND, 0644)` and two `write` calls (the text, then the newline). It creates the file if needed.

**Example**

```
$ ./prog todo.txt buy-milk && cat todo.txt
buy-milk
```
