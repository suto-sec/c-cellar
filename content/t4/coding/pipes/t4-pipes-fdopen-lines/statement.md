# Lines through a FILE *

The child sends three lines through a pipe, using a `FILE *` made with `fdopen`: `alpha`, `beta`, `gamma`. The parent reads them with `fgets` (also on a `FILE *` made with `fdopen`) and prints each one numbered: `1: alpha`, `2: beta`, `3: gamma`.

**Example**

```
$ ./prog
1: alpha
2: beta
3: gamma
```
