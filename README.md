# c-cellar

A self-hosted lab to practise C. It runs in a container and you use it in your browser:

- **Coding exercises**: the statement on the left, VS Code on the right, and a **Check** button that compiles your program with `gcc -Wall -Wextra` and runs it against test cases, showing a diff when something is wrong. Two tracks: *C derusting* (you know C but have not used it for a while) and *C exercises* (exam-style programs of growing difficulty).
- **Theory**: questions in four formats (single choice, multiple choice, fill in the blank, drag to reorder) with an explanation after every answer.
- **Reference**: a searchable C reference (syntax, options, examples, common mistakes).
- **Readiness**: how prepared you are per topic, and which questions to review.

Everything you do is stored in the `.progress/` folder (delete it to start over). Nothing leaves your computer.

All content is tagged by course topic. Today there is only **`t3`**; more topics will be added as new tags.

## Run it

You need **podman** (rootless, preferred) or **docker**, and `bash`. On Arch: `sudo pacman -S podman`.

```bash
git clone https://github.com/suto-sec/c-cellar.git
cd c-cellar
./lab web          # first run builds the image (a few minutes) and opens http://localhost:8080
```

| you want to | type |
|---|---|
| start | `./lab web` |
| stop (progress is kept) | `./lab stop` |
| a shell in the lab environment (gcc, gdb, valgrind, `man`) | `./lab shell` |
| check that every exercise and all content is consistent | `./lab selftest` |
| see the logs | `./lab logs` |
| rebuild the image | `./lab build` |
| use other ports | `LAB_PORT=8090 LAB_VSCODE_PORT=8091 ./lab web` |
| force an engine | `LAB_ENGINE=docker ./lab web` |

Both ports listen on `127.0.0.1` only. The editor has no password because it is not reachable from other machines; do not publish those ports on a shared network. Student programs run with a time limit and CPU, memory and file-size limits in a throw-away folder, but it is **not** a security sandbox: run only code you wrote.

Installation notes for macOS and Windows will be added later; the launcher is plain bash and the image is plain Ubuntu 24.04.

## How it is organised

```
lab                      launcher (podman or docker)
container/               Containerfile (gcc, gdb, valgrind, man pages, code-server) and entrypoint
app/server.py            backend: content loader, exercise checker, grading, progress (Python, standard library only)
app/static/              the web interface (plain JS, no build step)
app/selftest.py          consistency checks used by `./lab selftest`
content/<tag>/           everything you study, per topic tag
  coding/<track>/<id>/   one folder per exercise
  theory/<set>.json      question sets
  reference/<area>.json  reference entries
.progress/               your progress, your answers, VS Code settings (git-ignored)
```

Every content id starts with its tag (`t3-d01-hello-args`, `t3-ptr-04`, `t3-ref-malloc`) and lives under `content/<tag>/`; `selftest` enforces both.

### An exercise (`content/t3/coding/<track>/<id>/`)

| file | |
|---|---|
| `meta.json` | `id`, `tag`, `track` (`derusting` or `exercises`), `topic`, `title`, `difficulty` 1-3, `order`, `summary`, optional `hints[]` and `info` |
| `statement.md` | what to write |
| `starter.c` | the file the student starts from (copied to `.progress/workspace/<id>/answer.c`) |
| `solution.c` | reference solution (shown by *Show solution*; `selftest` proves it passes) |
| `tests.json` | `{"cases": [...]}`, see below |
| `harness.c`, `*.h` | optional: a hidden `main` linked with the student's `answer.c`; headers are also copied next to `answer.c` |
| `fixtures/` | optional files copied into every test's working directory |

A test case: `name`, then either `args` (+ optional `stdin`) or a shell `cmd` (`$BIN` is the compiled program), optional `setup` (shell, run first), `stdout` (expected, compared with `match`: `exact` (default), `trim`, `sorted`, `regex`, `contains`), `exit` (`0` by default, or an integer, `"nonzero"`, `"any"`), `stderr_empty`, `timeout`, `hidden`.

### Theory questions (`content/t3/theory/*.json`)

A set has `id`, `tag`, `title`, `topic`, `order`, `questions[]`. Each question has `id`, `type`, `prompt`, optional `code`, `explain`, `difficulty`, and by type:

- `single`: `options[]`, `answer` (index), optional `option_notes[]`
- `multiple`: `options[]`, `answer` (list of indices), optional `option_notes[]`
- `fill`: `{{0}}`, `{{1}}`… markers in `prompt` and `blanks[]` (each a list of accepted answers, or `{"accept": [...], "ignore_space": true, "case": true}`; an answer starting with `re:` is a regular expression)
- `order`: `items[]` in the CORRECT order (the interface shuffles them)

A question whose answer depends on what a program prints can carry `verify: {"source": "<C program>", "stdout": "..."}`; `selftest` compiles and runs it, so the explanation cannot drift from the truth.
