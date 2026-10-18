# Multiply two matrices

The input of `matmul` (standard input) is two matrices of integers, one after the other. Each one is written as its size `ROWS COLS` followed by its `ROWS` lines of `COLS` numbers (everything separated by white space). Sizes are positive and can be large, so allocate with `malloc`/`calloc`.

Print the product `A x B`: one line per row, numbers separated by single spaces.

- If the number of columns of `A` is not the number of rows of `B` print `matmul: dimension mismatch` to standard error and exit with status 1.
- If the input is incomplete print `matmul: bad input` to standard error and exit with status 1.

Use a `struct` for a matrix and functions to create, multiply and free matrices.

**Example**

```
$ printf '2 3\n1 2 3\n4 5 6\n3 2\n7 8\n9 10\n11 12\n' | ./prog
58 64
139 154
```
