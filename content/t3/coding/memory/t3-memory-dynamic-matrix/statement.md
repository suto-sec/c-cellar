# A matrix on the heap

Read `r c` (1 to 100 each). Allocate a matrix of `r` rows and `c` columns **dynamically** (an array of `r` pointers, each to a row of `c` ints). Fill the element at row `i`, column `j` with `i * c + j` and print the matrix, one row per line, numbers separated by single spaces. Free everything.

**Example**

Input:
```
2 3
```
Output:
```
0 1 2
3 4 5
```
