# factory: two stores and an assembler

`prog CHASSIS_SLOTS ENGINE_SLOTS CARS` simulates a small car factory with **three threads**:

- the **chassis maker** puts `CARS` chassis, one by one, into the chassis store, which has room for `CHASSIS_SLOTS` of them;
- the **engine maker** puts `CARS` engines, one by one, into the engine store, which has room for `ENGINE_SLOTS`;
- the **assembler** takes one chassis and one engine from the stores and makes a car, `CARS` times.

A maker that finds its store full waits; the assembler waits when a store is empty. The sizes of the stores are only known at run time: reserve them with `malloc`. The program must not deadlock, and at the end it must **join** the three threads, release the memory and the synchronisation objects, and print:

```
cars: C
store limits respected: yes
left over: 0 chassis, 0 engines
Factory closed
```

`C` is the number of cars the assembler made. `store limits respected` is `yes` if neither store ever held more items than its size, and `no` otherwise.

**Example**

```
$ ./prog 5 3 2000
cars: 2000
store limits respected: yes
left over: 0 chassis, 0 engines
Factory closed
```
