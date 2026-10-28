# Ping, pong, ping, pong

`prog N` (with `N` from 1 to 20) creates two threads. One prints `ping` `N` times, the other `pong` `N` times, and they must **alternate**, starting with `ping`. Use semaphores to take turns.

**Example**

```
$ ./prog 2
ping
pong
ping
pong
```
