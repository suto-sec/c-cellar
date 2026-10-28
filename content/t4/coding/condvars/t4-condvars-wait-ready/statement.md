# Wait until it is ready

The main thread creates a worker thread and **waits** (mutex + condition variable, no `sleep`, no busy loop) until the worker says it is ready.

- The worker prints `worker: ready`, sets a global flag `ready` to `1` and signals the condition.
- The main thread, once the flag is set, prints `main: go`.

**Example**

```
$ ./prog
worker: ready
main: go
```
