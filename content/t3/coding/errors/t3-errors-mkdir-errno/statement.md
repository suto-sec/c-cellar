# Tell the errors apart

`mkdirreport DIR` tries `mkdir(DIR, 0755)` and prints exactly one of: `created` (success), `exists` (`errno` is `EEXIST`), `no parent` (`ENOENT`), `denied` (`EACCES`), or `error: <strerror text>` for any other reason.

**Example**

```
$ ./prog photos
exists
```
