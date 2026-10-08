prog: main.c libcalc.so calc.h
	gcc -Wall -Wextra -o prog main.c -L. -lcalc

libcalc.so: calc.c calc.h
	gcc -Wall -Wextra -shared -fPIC -o libcalc.so calc.c
