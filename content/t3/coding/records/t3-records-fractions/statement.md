# Fraction calculator

Each line of **standard input** is an operation between two fractions: `A/B OP C/D`, where `OP` is `+`, `-`, `*` or `/`. For each line print the result as a **reduced** fraction `N/D` (denominator positive, the sign on the numerator). If the denominator of the result is 1 print only the numerator. Zero is printed as `0`.

Print `error` for a line that is malformed, has a zero denominator, or divides by a fraction equal to zero. Blank lines are skipped. Define a `struct` for a fraction and one function per operation. Numbers fit in a `long` and so do the intermediate results.

**Example**

```
1/2 + 1/3
3/4 * 2/3
1/2 / 0/5
4/2 - 2/1
```
prints
```
5/6
1/2
error
0
```
