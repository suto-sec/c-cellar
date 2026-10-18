# Temperature menu

Write a program with this menu. It must keep asking until the user chooses `0`, or the input ends:

```
1. Celsius to Fahrenheit
2. Fahrenheit to Celsius
0. Exit
Option:
```

- Print the whole menu (including the line `Option:`) **every time**, then read the option with `scanf`.
- For option 1 print `Celsius:`, read a number (it can have decimals) and print `<value> F` with one decimal. For option 2 print `Fahrenheit:` and print `<value> C` with one decimal.
- Any other option prints `Invalid option`.
- On option 0 print `Bye` and stop. If the input ends, stop without printing anything else.
- Write each conversion as a function, e.g. `float c_to_f(float)`.

F = C * 9 / 5 + 32.

**Example** (input `1`, `100`, `0`)

```
1. Celsius to Fahrenheit
2. Fahrenheit to Celsius
0. Exit
Option:
Celsius:
212.0 F
1. Celsius to Fahrenheit
2. Fahrenheit to Celsius
0. Exit
Option:
Bye
```
