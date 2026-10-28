# Is the mutex free?

1. The main thread locks a mutex.
2. It creates a thread. The thread calls `pthread_mutex_trylock` once and prints `trylock while locked: busy` if it returns `EBUSY` (or `trylock while locked: ok` if it got the mutex, which it never should; in that case it unlocks it again).
3. The main thread joins the thread and unlocks the mutex.
4. The main thread calls `pthread_mutex_trylock` itself and prints `trylock after unlock: ok` (`0`) or `trylock after unlock: busy`, then unlocks if it got it.

**Example**

```
$ ./prog
trylock while locked: busy
trylock after unlock: ok
```
