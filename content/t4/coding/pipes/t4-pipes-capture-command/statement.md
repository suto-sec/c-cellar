# Capture a command's output

`prog CMD [ARGS...]` runs the command in a child whose standard output goes into a **pipe** (not to the screen). The parent reads everything the command writes and prints it again line by line, each line preceded by `> ` (greater-than and a space). The parent waits for the child.

**Examples**

```
$ ./prog echo hi
> hi
$ ./prog printf 'a\nb\n'
> a
> b
```
