# Fixed-width integers

Include `<stdint.h>`. Read one integer `n` (from 0 to 3000000000). Keep it in an `int64_t` and print `square = <n * n>` computed with that type (it does not fit in an `int`). Then store `n` in a `uint8_t` and print `low byte = <that value>`: an 8-bit unsigned type keeps only the remainder of `n` divided by 256.

**Example**

Input:
```
300
```
Output:
```
square = 90000
low byte = 44
```
