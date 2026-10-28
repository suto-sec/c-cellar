# aleatorios: a random number on request

Write a program with a parent and a child:

- the child's job is to generate random numbers from 0 to 10: **each time it receives the signal `SIGUSR1`** from the parent, it writes one random number (as a line of text, `7\n`) into a pipe;
- the parent reads lines from standard input; **for every line** it asks the child for a number (sending `SIGUSR1`), reads the number from the pipe and prints it on its own line;
- when the input ends, the parent sends `SIGTERM` to the child, waits for it and ends.

The contents of the lines the parent reads do not matter, only how many there are.

**Example**

```
$ printf 'x\ny\nz\n' | ./prog
7
1
6
```
