# line-batches: children that wait for a signal

`prog FILE K` counts the lines of a text file in **batches of `K` lines** (`K` from 1 to 255), using one child per batch and **no pipes**.

The parent opens `FILE` and, one at a time, creates children numbered from 1. Each child stays **paused until the parent sends it `SIGUSR1`**; then it reads the next `K` lines of the file (or fewer, if the file ends first), prints `Process I: read L lines` and ends with `L` as its exit status. The parent waits for it, adds `L` to the total and, if `L` was `K`, creates the next child. When a child reads fewer than `K` lines the parent prints `Parent: the file has T lines`.

A line is a piece of text that ends with a newline. If the file has an exact multiple of `K` lines, the last child reads 0 lines.

**Example** (a file with 54 lines)

```
$ ./prog salary.txt 20
Process 1: read 20 lines
Process 2: read 20 lines
Process 3: read 14 lines
Parent: the file has 54 lines
```
