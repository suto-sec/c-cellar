# The maximum, four threads

Read `N` (at most 1000, at least 1) and `N` integers. **Four threads** each look for the maximum of their own quarter of the numbers (some quarters are empty when `N` is below 4). The main thread takes the largest of the four results and prints `max: M`.

**Examples**

```
$ echo 5 3 9 -2 7 1 | ./prog
max: 9
$ echo 3 -3 -1 -2 | ./prog
max: -1
```
