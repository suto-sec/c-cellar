# Did the command run?

`prog CMD [ARGS...]` runs the command in a child, like a shell would, and then reports what happened:

- if the command could not be started (the child's `exec` failed and it ended with status `127`): `command not found`;
- otherwise: `exit status N`, with the command's own status.

**Examples**

```
$ ./prog true
exit status 0
$ ./prog false
exit status 1
$ ./prog sh -c 'exit 3'
exit status 3
$ ./prog nosuchcommand
command not found
```
