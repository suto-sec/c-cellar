# A static library

Write the module **`strutil.h`** / **`strutil.c`** with `int count_char(const char *s, char c);` (how many times `c` appears in `s`) and a **`Makefile`** that:

1. compiles `strutil.c` into `strutil.o`;
2. archives it with `ar rcs libstrutil.a strutil.o`;
3. links the hidden `main.c` with the library into `prog` (`gcc -o prog main.c -L. -lstrutil`).

`make` must produce `prog` (the build command is just `make`). Recipe lines start with a TAB.
