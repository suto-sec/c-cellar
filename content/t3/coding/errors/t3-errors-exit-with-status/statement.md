# Choose the exit status

`status N` ends the program with the exit status `N` (0 to 255) and prints nothing. With no argument exit with 0. (The shell shows the status of the last command in `$?`.)

**Example**

```
$ ./prog 7; echo $?
7
```
