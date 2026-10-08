# Total of a CSV file

`csvtotal FILE` reads lines of the form `name,price` (the name has no commas or spaces; the price is a number) and prints `items=<count> total=<sum of the prices with two decimals>`. Lines that do not have this form are ignored.

**Example**

```
$ ./prog f.csv
items=2 total=4.50
```
