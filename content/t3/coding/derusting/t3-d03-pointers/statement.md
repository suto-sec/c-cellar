# Swap and output parameters

`main` (provided, in a hidden file) reads `n` and then `n` integers, calls your functions and prints the result.
Implement these two functions in `answer.c`:

```c
void swap(int *a, int *b);                        /* exchange the two ints */
void minmax(const int *v, int n, int *min, int *max);  /* smallest and largest of v[0..n-1], n >= 1 */
```

Expected behaviour of the provided `main`:

```
$ echo "4  7 -2 9 3" | ./prog
min=-2 max=9
swapped: 9 -2
```
