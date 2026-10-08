# Remember every word

Read words (separated by spaces or newlines) until the input ends, with **no limit** on how many. Print them in reverse order, one per line. Store each word in memory you allocate (`strdup`) and grow the array of pointers with `realloc`.

**Example**

Input:
```
one two three
```
Output:
```
three
two
one
```
