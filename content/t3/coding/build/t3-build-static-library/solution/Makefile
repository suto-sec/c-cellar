prog: main.c libstrutil.a strutil.h
	gcc -Wall -Wextra -o prog main.c -L. -lstrutil

libstrutil.a: strutil.o
	ar rcs libstrutil.a strutil.o

strutil.o: strutil.c strutil.h
	gcc -Wall -Wextra -c strutil.c
