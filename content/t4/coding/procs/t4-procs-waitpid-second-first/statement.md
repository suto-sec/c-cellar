# Wait for a particular child

The parent creates two children, one after the other: the first ends with status `10`, the second with status `20`.

The parent collects the **second** child first with `waitpid`, printing `second: 20`, and then the first one, printing `first: 10`.

**Example**

```
$ ./prog
second: 20
first: 10
```
