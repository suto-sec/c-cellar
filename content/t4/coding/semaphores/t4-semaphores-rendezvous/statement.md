# Wait for each other

Two threads, A and B, each do three things: print `A before` (or `B before`), **meet the other thread**, and print `A after` (or `B after`). Neither may print its `after` line until **both** have printed their `before` line. Use two semaphores.

The order inside each pair is free, but the first two lines are the `before` lines and the last two are the `after` lines.

**Example**

```
$ ./prog
B before
A before
A after
B after
```
