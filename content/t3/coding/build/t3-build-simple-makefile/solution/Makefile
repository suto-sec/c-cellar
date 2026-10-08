CC = gcc
CFLAGS = -Wall -Wextra

prog: main.o util.o
	$(CC) -o prog main.o util.o

%.o: %.c util.h
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o prog
