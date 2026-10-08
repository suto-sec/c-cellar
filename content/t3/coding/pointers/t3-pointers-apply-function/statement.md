# A function as an argument

A hidden `main` reads `n` and `n` integers and a word (`double` or `negate`), then calls your function with the matching operation and prints the array. Write in `answer.c`:

```c
void apply(int *v, int n, int (*f)(int));   /* replaces every element x by f(x) */
```

**Example**

Input:
```
3
1 2 3
double
```
Output:
```
2 4 6
```
