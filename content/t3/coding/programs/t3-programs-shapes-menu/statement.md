# A shapes calculator

Read commands from standard input, one per line, until `quit` or the end of the input:

- `circle R` prints the area of a circle (use pi = 3.14159265);
- `rectangle W H` prints the area of a rectangle;
- `triangle B H` prints the area of a triangle (base times height divided by 2);
- anything else prints `unknown command`.

Each result is printed as `area=<value with two decimals>` (numbers can have decimals). No prompts.

**Example**

Input:
```
circle 1
rectangle 3 4
triangle 5 6
quit
```
Output:
```
area=3.14
area=12.00
area=15.00
```
