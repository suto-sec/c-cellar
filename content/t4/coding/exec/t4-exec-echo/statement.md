# Run echo in a child

The parent creates a child. The child replaces itself with the command `echo hello world` using `execlp`. The parent waits for it and then prints `done`.

**Example**

```
$ ./prog
hello world
done
```
