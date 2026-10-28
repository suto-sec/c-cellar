# Three commands in a chain

Do what `ls | sort -r | head -n 2` does, with no shell: **three** child processes joined by **two** pipes. The parent waits for all three.

The program prints the two names that come last in reverse alphabetical order.

**Example**

```
$ ls
a  b  c  d
$ ./prog
d
c
```
