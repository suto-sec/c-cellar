# A bank account

Read commands from standard input, one per line, until the input ends. The account starts at 0.

- `deposit N`: add `N` to the balance and print `ok`;
- `withdraw N`: if `N` is more than the balance print `insufficient funds` (and change nothing), otherwise subtract it and print `ok`;
- `balance`: print `balance: <amount>`;
- anything else: print `unknown command`.

`N` is a non-negative integer. Use a `struct account` and functions to do the work.

**Example**

Input:
```
deposit 100
withdraw 30
balance
```
Output:
```
ok
ok
balance: 70
```
