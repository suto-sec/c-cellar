# Modify a struct through a pointer

The header `person.h` (in your folder) defines `struct person { char name[32]; int age; };` and declares:

```c
void birthday(struct person *p);   /* makes the person one year older */
```

Write it in `answer.c` (start with `#include "person.h"`). A hidden `main` reads `name age`, calls it twice and prints `<name> is now <age>`.

**Example**

Input:
```
Grace 30
```
Output:
```
Grace is now 32
```
