# A contact book

Write a program that keeps a list of contacts (name and phone number, both words of at most 31 characters without spaces) in memory and reads **commands** from standard input, one word at a time (arguments follow their command, separated by white space), until `quit` or the end of the input. There is no limit on the number of contacts. The list keeps the order in which contacts were added.

| command | does | prints |
|---|---|---|
| `add NAME PHONE` | adds a contact | `added`, or `exists` if a contact with that name is already there (nothing is changed) |
| `find NAME` | looks a contact up | `NAME: PHONE`, or `not found` |
| `del NAME` | deletes the contact | `deleted`, or `not found` |
| `list` | shows all, in order | one `NAME PHONE` line per contact, or `(empty)` |
| `count` | | `count: N` |
| `quit` | stops | |
| anything else | (only that word is consumed) | `unknown command` |

**Example** (input: `add ana 111 add bob 222 find bob del ana list quit`)

```
added
added
bob: 222
deleted
bob 222
```
