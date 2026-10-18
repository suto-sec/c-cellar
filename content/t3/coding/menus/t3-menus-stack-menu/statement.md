# A stack with a menu

Keep a **stack of at most 5 integers** in an array. Read options from standard input (separated by spaces or newlines) until option `0` or the end of the input. Print no prompts, only the results:

| option | what it does | prints |
|---|---|---|
| `1 N` | push the number `N` (the option is followed by its value) | nothing, or `stack full` if there are already 5 values (the value is dropped) |
| `2` | pop the top value | `popped N`, or `stack empty` |
| `3` | look at the top value without removing it | `top N`, or `stack empty` |
| `4` | the number of values in the stack | `size K` |
| `0` | stop | nothing |
| anything else | | `invalid option` |

Write one function per operation.

**Example** (input: `1 10 1 20 3 2 2 2 4 0`)

```
top 20
popped 20
popped 10
stack empty
size 0
```
