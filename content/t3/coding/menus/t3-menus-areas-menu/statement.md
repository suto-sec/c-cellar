# Areas menu

Write a program that shows this menu and repeats until the user chooses the exit option (or the input ends):

```
1. Triangle area
2. Rectangle area
3. Square area
4. Circle area
5. Exit
Option:
```

Print the whole menu **every time**, then read the option with `scanf`. Then:

- `1`: print `Base:`, read a number, print `Height:`, read a number, and print `Area = <value>`.
- `2`: print `Width:` then `Height:` (reading a number after each) and print `Area = <value>`.
- `3`: print `Side:`, read it and print `Area = <value>`.
- `4`: print `Radius:`, read it and print `Area = <value>` (pi = 3.14159265).
- `5`: print `Bye` and stop. Any other option prints `Invalid option`.

Numbers can have decimals; every area is printed with two decimals. Write each calculation as a **function that receives its parameters** (like `float calculo(float, float);`), not code inside `main`.

**Example** (input `1`, `3`, `4`, `5`)

```
1. Triangle area
2. Rectangle area
3. Square area
4. Circle area
5. Exit
Option:
Base:
Height:
Area = 6.00
1. Triangle area
2. Rectangle area
3. Square area
4. Circle area
5. Exit
Option:
Bye
```
