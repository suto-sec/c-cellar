# Towers of Hanoi: count the moves

Moving `n` discs from one peg to another takes: move `n-1` discs aside, move the biggest disc, move the `n-1` discs on top of it. A hidden `main` reads `n` and prints `hanoi(n) = <moves>`. Write in `answer.c`, with recursion:

```c
long hanoi(int n);   /* number of moves for n discs; hanoi(0) is 0 */
```

**Example**

Input:
```
3
```
Output:
```
hanoi(3) = 7
```
