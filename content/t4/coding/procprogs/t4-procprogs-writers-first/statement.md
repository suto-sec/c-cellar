# writers-first: readers and writers

Four threads use a shared resource, each doing one operation. A **reader** can share the resource with other readers; a **writer** needs it for itself. **Writers have priority**: a reader may only start if nobody is writing **and no writer is waiting**.

Every thread first sleeps (`usleep`) until its own start time, then performs its operation, which takes some time (`usleep` too):

| thread | role | starts at | operation takes |
|---|---|---|---|
| `R0` | reader | 0 ms | 300 ms |
| `W1` | writer | 100 ms | 100 ms |
| `R2` | reader | 150 ms | 50 ms |
| `R3` | reader | 220 ms | 50 ms |

Use **one mutex and a condition variable**. The program joins the threads and prints three lines:

- `start order: ` the names in the order in which the operations **started** (separated by single spaces),
- `max readers at once: N`, the highest number of readers that were reading at the same time,
- `violations: V`, how many times a thread found the resource in a forbidden state when it started (a reader that finds somebody writing, or a writer that finds somebody reading or writing). It must be 0.

With priority for the writers, `W1` has to wait for `R0` (a writer needs the resource alone), but `R2` and `R3`, who arrive while `W1` is waiting or writing, must wait for `W1` even though `R0` alone would have let them in. When `W1` ends both readers read together. `R2` and `R3` can start in either order.

**Example**

```
$ ./prog
start order: R0 W1 R2 R3
max readers at once: 2
violations: 0
```
