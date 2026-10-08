# A shared library

Write the module **`calc.h`** / **`calc.c`** with `int square(int x);` and a **`Makefile`** that builds a **dynamic** library `libcalc.so` (`gcc -shared -fPIC -o libcalc.so calc.c`) and links the hidden `main.c` against it, producing `prog` (`gcc -o prog main.c -L. -lcalc`). The build command is `make`.

The program only runs if the loader can find the library: the tests run it with `LD_LIBRARY_PATH=.`.
