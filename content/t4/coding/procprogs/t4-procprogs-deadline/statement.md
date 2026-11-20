# deadline: abort the children after a time limit

`prog SECONDS DELAY FILE` creates two children joined by a pipe:

- the **producer** reads `FILE` and writes its content into the pipe, then *sleeps `DELAY` seconds* (it is a slow producer) and only then closes the pipe and ends;
- the **consumer** reads from the pipe until the end of the data and prints `characters: N`, the number of bytes it received.

The parent waits for them, but **at most `SECONDS` seconds**. If both children end in time it prints `finished in time`. Otherwise it **kills both children** with `SIGKILL`, collects them and prints `timeout: children aborted`.

If the producer is killed first, the consumer may see the end of the data and print its line before it is killed too; the tests accept that. They never use `SECONDS` equal to `DELAY`.

**Examples**

```
$ printf 'hello\nworld\n' > d.txt
$ ./prog 3 1 d.txt
characters: 12
finished in time
$ ./prog 1 5 d.txt
timeout: children aborted
```
