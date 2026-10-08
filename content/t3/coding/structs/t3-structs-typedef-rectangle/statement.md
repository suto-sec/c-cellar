# A typedef'd rectangle

Define, with `typedef`, a type `Trect` for a rectangle with the `int` members `width` and `height`. Write a function `Trect scale(Trect r, int k)` that returns the rectangle with both sides multiplied by `k`, and an `int area(Trect r)`.

The input has lines with three integers `width height k`, until the input ends. For each, build the rectangle, scale it, and print `<width> x <height>, area <area>` of the **scaled** one.

**Example**

Input:
```
3 4 2
```
Output:
```
6 x 8, area 48
```
