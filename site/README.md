# c-cellar, browser edition

A static site (GitHub Pages) with all the content of c-cellar and a small Linux computer inside the page that compiles and runs your C with
real gcc (so `fork`, pipes, signals and threads work too). No server. Progress is stored in the browser and can be exported / imported as a file.
Not in this edition: the daily challenge and the readiness page (they need the Python server of the lab version).

```
build_site.py     data + page       python3 site/build_site.py [--bundle --out dist-test]   -> dist/
fetch_parts.sh    downloads the machine's parts (pinned versions + checksums) into work/
build_vm.sh       packs the machine  bash site/build_vm.sh                                  -> dist/vm/ (+ NOTICE.txt with the licences)
overlay/          the two files of our own that go into the machine (boot script, lab-compile)
overrides.json    the few tests whose expected text is glibc-specific (the machine is musl): the command differs, never what is checked
pages.yml         the GitHub Actions workflow that builds and deploys the site (goes in .github/workflows/)
src/              the site's own code: tar.js runner.js api.js boot.js vm.js panels.js site.css
tools/            verify_native.mjs  vm_smoke.mjs  vm_parity.sh
tests/            grade.test.mjs  api.test.mjs  page_vm.test.mjs  make_grade_vectors.py  e2e/*.json (browser-run specs)
work/ dist/ dist-test/ out/   generated, not committed (.gitignore)
```

## How it works
- `build_site.py` uses the repo's own loaders (`app/server.py`), so the data is what the app shows. It patches a copy of `app/static/app.js`
  (list `APP_PATCHES`; the build stops if a patch no longer applies). The repo's own app.js is untouched.
- `window.CELLAR_API` (`src/api.js`) answers every call app.js used to send to the server, from `data/*.json` and `localStorage`. The theory
  grading is a port of the server's.
- Checking an exercise: `runner.js` makes a job (a tar with the sources, the tests and bash scripts); the machine (`vm.js`: the v86 x86 emulator
  with a 32-bit Alpine Linux and gcc) unpacks it, builds, runs every case and sends back a tar of outputs; `runner.js` judges them with the same
  rules as the server. The first Check starts the machine (about 15 s, plus a one-time download of about 60 MB for the compiler, which the browser keeps).
- The machine is 32-bit (`long` is 4 bytes) and musl, not glibc. Checks take 10-20 s each.

## Verified (here, with node and a headless Chromium through `browser-run`, file:// build with the machine replaced by a mock)
| check | result |
|---|---|
| `node tools/verify_native.mjs`: all 382 reference solutions through the browser's job format, with this computer's bash/gcc | 382 pass |
| same with `--mutate`: an empty program must fail every exercise | all 382 fail |
| `node tests/grade.test.mjs`: JS theory grading vs the server's, 2433 responses | 0 differences |
| `node tests/api.test.mjs`: index + 382 exercise pages vs the real server; check/skip/solution/theory/favorites/reset flow | 0 differences |
| `node tests/page_vm.test.mjs`: the page's own code (vm.js, api.js, runner) with the real machine in node: wrong program fails, reference passes, Compile & run | passes |
| `node tools/vm_smoke.mjs --all` / `bash tools/vm_parity.sh 3`: every reference solution through the real machine | 373 pass; 9 are marked "cannot be checked here" (they need a 64-bit `long`) |
| `browser-run tests/e2e/*.json`: home, coding list, an exercise (type, Check, Hint), theory answer, reference, settings render in Chromium | pass |

## What differs from the lab version
- **Nine exercises cannot be graded** (`overrides.json` `_unavailable`): they need a 64-bit `long` (type sizes, big sums, 20!, hanoi, 2147483647 as a prime...) and the
  machine is 32-bit. They show a note, Compile and run still work, Check says it cannot grade them.
- One test command of the shared-library exercise reads musl's message instead of glibc's. Time limits are 6x the lab's (the machine is emulated).
- No daily challenge and no readiness page (they need the Python server).

## Verified in a real browser
The owner opened `/?autotest=t3-output-hello-world` from `python3 -m http.server -d site/dist 8000`: the machine booted in the page, gcc compiled the
reference solution, it ran and was graded: `{"passed":true,"compiled":true,"seconds":18.1}` (screenshot 2026-10-30). Still not tried by hand in a
browser: a long session in the editor + terminal panels with the real machine (the panels were checked in Chromium with the machine mocked).

## Publishing
1. The site lives in the c-cellar repo as `site/` (source only, ~100 KB). `.github/workflows/pages.yml` (this folder's `pages.yml`) builds the
   machine and the site on GitHub's servers and deploys it, so no binaries are committed (the machine is ~200 MB: 1715 files, the biggest 42 MB;
   Pages allows 100 MB per file and 1 GB per site).
2. In the repository: Settings > Pages > Build and deployment > Source: **GitHub Actions**. The site is then at `https://<user>.github.io/c-cellar/`.
3. Licences: `dist/vm/NOTICE.txt` (made by `build_vm.sh`) lists every package with its licence and where the source is (Alpine aports); the page
   footer links to it. The reference solutions and hidden tests are in the bundle (they are public in the repo anyway).
