# Is it still there?

The parent creates a child that ends at once (`_exit(0)`). The parent then:

1. sleeps one second (`sleep(1)`), so the child has surely ended;
2. checks whether the child still exists with `kill(pid, 0)` and prints `before wait: still there` or `before wait: gone`;
3. collects the child with `waitpid`;
4. checks again and prints `after wait: still there` or `after wait: gone`.

`kill(pid, 0)` sends no signal at all: it only tells you whether the process exists.

**Example**

```
$ ./prog
before wait: still there
after wait: gone
```
