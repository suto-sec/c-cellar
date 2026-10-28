# Bounded buffer with conditions

`prog N` again runs a producer and a consumer over a **circular buffer of 5 slots**: the producer puts `1..N`, the consumer takes `N` numbers and adds them up. This time use **one mutex and two condition variables** (`has_room` and `has_data`), no semaphores.

Keep the number of items in the buffer, `count`. Record the largest value it reaches, `max_count`. The main thread prints two lines: `sum = S` and `overflow: no` (or `overflow: yes` if `max_count` ever exceeded 5; it must not).

**Example**

```
$ ./prog 1000
sum = 500500
overflow: no
```
