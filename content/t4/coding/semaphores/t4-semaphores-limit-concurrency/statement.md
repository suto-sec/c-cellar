# Only two at a time

Six threads want to use a room that holds **two** at a time (a counting semaphore initialised to `2`). Each thread: enters the room, stays inside for one second (`sleep(1)`), and leaves.

Keep a counter `inside` of the threads inside the room (protected by a mutex) and the largest value it ever had, `max_inside`. After joining the six threads, print `max inside: M`. If your semaphore works, `M` is exactly `2` (and the program takes about three seconds).

**Example**

```
$ ./prog
max inside: 2
```
