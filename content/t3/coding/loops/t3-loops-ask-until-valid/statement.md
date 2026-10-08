# Ask until it is valid

Read integers from the input until you get one between 1 and 10 (both included). Print `try again` for every number that is outside that range, and finally `accepted <n>` for the valid one. Use a **`do ... while`** loop. If the input ends before a valid number appears, exit with status 1.

**Example**

Input:
```
15
0
7
```
Output:
```
try again
try again
accepted 7
```
