# Private state in a module

Write the module **`ids.h`** / **`ids.c`** that hands out identifiers. `int next_id(void)` returns 1 the first time, 2 the second time, and so on. The counter must be a **`static` variable at file level** in `ids.c` (invisible to other files). A hidden `main.c` calls `next_id()` four times and prints the values.
