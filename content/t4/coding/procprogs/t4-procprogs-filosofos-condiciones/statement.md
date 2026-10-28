# filosofos: with condition variables

The same dining philosophers (5 of them, 3 meals each, philosopher `i` needs chopsticks `i` and `(i + 1) % 5`), solved this time with **one mutex and condition variables** (no semaphores).

Keep the state of every philosopher (thinking, hungry, eating). A hungry philosopher may start eating only when **neither neighbour is eating**; otherwise it waits on the condition (releasing the mutex). When a philosopher finishes eating, it wakes the others. Guarantee that no deadlock is possible.

**Output**: the philosophers print **nothing** while they run. After joining all the threads, the main thread prints one line per philosopher, in order, `philosopher I ate 3 times`, and a last line `total meals: 15`.

A philosopher that never gets both chopsticks makes the program hang, so a wrong solution is caught by the time limit.

**Example**

```
$ ./prog
philosopher 0 ate 3 times
philosopher 1 ate 3 times
philosopher 2 ate 3 times
philosopher 3 ate 3 times
philosopher 4 ate 3 times
total meals: 15
```
