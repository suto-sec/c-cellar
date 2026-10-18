# Statistics of a vector

The input starts with a positive integer `N`, followed by `N` integers (the vector). Then come options, one per `scanf`, until `0` or the end of the input. Print no prompts, only the results:

| option | prints |
|---|---|
| `1` | `sum = <sum>` |
| `2` | `average = <average>` (two decimals) |
| `3` | `max = <largest>` |
| `4` | `min = <smallest>` |
| `5` | `sorted: ` followed by the numbers in ascending order, separated by single spaces |
| `0` | stop, no output |
| anything else | `invalid option` |

Write one function per option, receiving the vector and its length. `N` can be large: allocate with `malloc`.

**Example** (input `4`, `5 -2 9 1`, `1 3 5 0`)

```
sum = 13
max = 9
sorted: -2 1 5 9
```
