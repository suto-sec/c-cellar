# Write a Makefile

The sources `main.c`, `util.c` and `util.h` are already provided (hidden). Write a **`Makefile`** with:

- a default target `prog` that links `main.o` and `util.o` into `prog`;
- a rule to build any `.o` from its `.c` (a pattern rule `%.o: %.c`, using `$<`), with `-Wall -Wextra`;
- the objects must depend on `util.h`;
- a `clean` target that removes the objects and `prog`.

Remember: recipe lines start with a **TAB**.
