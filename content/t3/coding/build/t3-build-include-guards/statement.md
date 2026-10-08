# Include guards

A hidden `main.c` includes your header **twice** (as happens in real projects when two headers include the same one) and then uses a `struct counter`. Write:

- **`counter.h`**: define `struct counter { int value; };` and declare `void counter_inc(struct counter *c);` and `int counter_get(const struct counter *c);`. Without an include guard the double inclusion is a compile error (redefinition of `struct counter`).
- **`counter.c`**: define the two functions (`counter_inc` adds 1).
