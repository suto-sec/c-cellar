# Return a pointer to the maximum

A hidden `main` reads `n` and `n` integers, finds the maximum with your function, **sets it to 0 through the returned pointer** and prints the array. Write in `answer.c`:

```c
int *find_max(int *v, int n);   /* pointer to the (first) largest element of v[0..n-1] */
```

**Example**

Input:
```
4
3 8 2 5
```
Output:
```
3 0 2 5
```
