# A question and an answer

Read integers from standard input until the end. For each one, the **parent** sends it to a **child** through a pipe; the child sends its square back through a **second** pipe; the parent prints `N squared is S`.

Ask one number at a time (send, then wait for the reply). When the input ends, the parent closes its pipe so the child stops, and waits for it.

**Example**

```
$ echo 2 3 10 | ./prog
2 squared is 4
3 squared is 9
10 squared is 100
```
