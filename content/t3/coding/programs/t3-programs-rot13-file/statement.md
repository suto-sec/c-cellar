# ROT13 a file

`rot13 IN OUT` copies the file `IN` to `OUT` replacing every letter by the letter 13 places further in the alphabet (wrapping around, keeping upper/lower case; `a` becomes `n`, `z` becomes `m`). Other characters are copied unchanged. Applying it twice gives the original text. If `IN` cannot be opened print `cannot open IN` on standard error and exit with 1; if `OUT` cannot be created exit with 2.

**Example**

```
$ ./prog in.txt out.txt && cat out.txt
Uryyb
```
