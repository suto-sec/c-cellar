# mayusculas: shout it back

Write a program in which the parent reads lines from standard input and a child turns them into capitals:

- the parent creates a child and **two pipes** (one for each direction);
- the parent reads a line from its standard input and sends it to the child through the first pipe;
- the child reads it, turns every letter into upper case and sends it back through the second pipe;
- the parent prints on standard output what it receives from the child.

Handle one line at a time (send, then read the answer). When the input ends the parent closes its pipe, the child finishes, and the parent waits for it.

**Example**

```
$ printf 'hola\nmundo\n' | ./prog
HOLA
MUNDO
```
