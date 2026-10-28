# Who is my parent?

Write a program that checks the pid relations between a parent and its child.

1. Before forking, the parent remembers its own pid (`getpid`).
2. The child compares its parent's pid (`getppid`) with the remembered value and ends with status `0` if they are equal, `1` if not.
3. The parent waits for the child and prints `child sees the right parent: yes` when the child's status was `0`, and `child sees the right parent: no` otherwise.

**Example**

```
$ ./prog
child sees the right parent: yes
```
