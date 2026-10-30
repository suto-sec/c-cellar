/* The checker of the site: the same job as app/server.py check_exercise, split in two halves that do not know any VM.
 *   makeJob(ex, files, opts)  -> the tar to put in the machine (sources, tests, scripts) + the command that runs it
 *   evaluate(ex, raw)         -> the result object the page shows, from what the machine sent back (build log, outputs, exit codes)
 * Whatever runs the job (the v86 VM in the browser, or the local bash in the Node test) only has to: unpack the tar in /lab/job (or $J),
 * run `bash ctl/run.sh` there, and return the tar that run.sh leaves in results.tar. */
(function (root) {
  "use strict";
  const Tar = typeof require !== "undefined" ? require("./tar.js") : root.CellarTar;
  const enc = new TextEncoder();
  const dec = new TextDecoder();
  const PATH = "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin";

  const shq = (s) => (s === "" ? "''" : /^[\w@%+=:,./-]+$/.test(s) ? s : "'" + s.replace(/'/g, "'\\''") + "'");   // shlex.quote

  function b64bytes(s) {
    if (typeof Buffer !== "undefined") return new Uint8Array(Buffer.from(s, "base64"));
    const bin = atob(s);
    const out = new Uint8Array(bin.length);
    for (let i = 0; i < bin.length; i++) out[i] = bin.charCodeAt(i);
    return out;
  }

  /** The time limit of a case in seconds: the lab's (5 by default) times the machine's slowness; a case that runs the compiler (make, gcc) several times gets four times more. */
  const caseLimit = (c, scale) => Math.ceil((c.timeout || 5) * scale * (/\b(make|gcc|cc)\b/.test(c.cmd || "") ? 4 : 1));

  /** opts: timeScale (multiplies every time limit: the VM is slower than a real machine), ulimitCpu (seconds). */
  function makeJob(ex, files, opts = {}) {
    const scale = opts.timeScale || 1;
    const cpu = opts.ulimitCpu || 10 * scale;
    const entries = [];
    const add = (name, data, mode) => entries.push({ name, data, mode });
    // the program's folder: what the student wrote, the headers (always the original), the files it is linked with, a harness
    for (const n of ex.files) add("src/" + n, files[n] || "");
    for (const [n, t] of Object.entries(ex.headers || {})) add("src/" + n, t);
    for (const [n, t] of Object.entries(ex.hidden || {})) add("src/" + n, t);
    if (ex.harness) add("src/harness.c", ex.harness);
    // how it is built: the exercise's own build command, or the plain gcc line of the lab
    const build = ex.build || "gcc -std=gnu17 -Wall -Wextra -g -o prog answer.c" + (ex.harness ? " harness.c" : "") + " -pthread -lm";
    add("ctl/build.sh", build + "\n");
    // fixtures: the folder every test starts from (for a multi-file build it is a copy of the finished build: run.sh makes it)
    if (!ex.build) {
      for (const [rel, b64] of Object.entries(ex.fixtures.files)) add("fixtures/" + rel, b64bytes(b64));
      add("ctl/fixtures.dirs", ex.fixtures.dirs.join("\n") + "\n");
    }
    ex.tests.forEach((c, i) => {
      const limit = caseLimit(c, scale);
      const ulimit = `ulimit -t ${Math.max(cpu, limit)} -f 20480 -v 1048576 2>/dev/null; `;   // (the CPU cap never undercuts the case's own time limit)
      const env = Object.entries(c.env || {}).map(([k, v]) => `export ${k}=${shq(String(v))}\n`).join("");
      let invoke;
      if ("cmd" in c) {
        add(`ctl/c${i}.cmd`, ulimit + c.cmd + "\n");
        invoke = `bash "$J/ctl/c${i}.cmd"`;
      } else {
        add(`ctl/c${i}.cmd`, ulimit + 'exec "$@"\n');
        invoke = `bash "$J/ctl/c${i}.cmd" "$BIN" ` + (c.args || []).map((a) => shq(String(a))).join(" ");
      }
      if ("setup" in c) add(`ctl/c${i}.setup`, c.setup + "\n");
      add(`ctl/c${i}.in`, c.stdin || "");
      add(`ctl/c${i}.sh`, `#!/bin/bash
J=\${J:-/lab/job}
C="$J/w${i}"
rm -rf "$C"
if [ -d "$FIX" ]; then cp -a "$FIX" "$C"; else mkdir -p "$C"; fi
BIN="$J/src/prog"; [ -e "$C/prog" ] && BIN="$C/prog"
export PATH=${PATH} HOME="$C" BIN="$BIN" LANG=C.UTF-8 TMPDIR="$C"
${env}cd "$C"
${"setup" in c ? `timeout -s KILL ${10 * scale} bash "$J/ctl/c${i}.setup" >/dev/null 2>&1\n` : ""}s=$(date +%s%N)
timeout -s KILL ${limit} ${invoke} < "$J/ctl/c${i}.in" > "$J/r/${i}.out" 2> "$J/r/${i}.err"
echo $? > "$J/r/${i}.rc"
e=$(date +%s%N); echo $(( (e - s) / 1000000 )) > "$J/r/${i}.ms"
`, 0o755);
    });
    add("ctl/run.sh", `#!/bin/bash
J=\${J:-/lab/job}
export J
cd "$J" || exit 1
mkdir -p r
(cd src && bash "$J/ctl/build.sh") > build.log 2>&1
echo $? > build.rc
[ -x src/prog ] && touch r/prog.ok
if [ "$(cat build.rc)" = 0 ] && [ -f r/prog.ok ]; then
${ex.build ? '  rm -rf _build; mkdir _build; for f in src/*; do [ -f "$f" ] && cp -p "$f" _build/; done\n  FIX="$J/_build"' : '  while read -r d; do [ -n "$d" ] && mkdir -p "$J/fixtures/$d"; done < ctl/fixtures.dirs\n  FIX="$J/fixtures"'}
  export FIX
  for i in $(seq 0 ${ex.tests.length - 1}); do bash "$J/ctl/c$i.sh"; done
fi
tar -cf results.tar r build.log build.rc
`, 0o755);
    return { tar: Tar.create(entries), command: "bash ctl/run.sh", cases: ex.tests.length };
  }

  // ------------------------------------------------------------------ evaluate: the same rules as app/server.py (run_case, compare)

  function compare(got, want, mode) {
    const hints = [];
    if (mode === "exact") {
      if (got === want) return [true, hints];
      const lines = (t) => t.split("\n").map((l) => l.trimEnd());
      const words = (t) => t.split(/\s+/).filter(Boolean);
      const sortedLines = (t) => t.split("\n").sort().join("\n");
      if (got.trimEnd() === want.trimEnd()) hints.push("only trailing whitespace/newlines differ (missing or extra final newline?)");
      else if (lines(got).join("\n") === lines(want).join("\n")) hints.push("only trailing spaces at the end of lines differ");
      else if (words(got).join(" ") === words(want).join(" ")) hints.push("same words, different spacing or line breaks");
      else if (sortedLines(got) === sortedLines(want)) hints.push("same lines, but in a different order");
      else if (got.toLowerCase() === want.toLowerCase()) hints.push("differs only in upper/lower case");
      return [false, hints];
    }
    if (mode === "trim") return [got.trim() === want.trim(), hints];
    if (mode === "sorted") return [got.split("\n").sort().join("\n") === want.split("\n").sort().join("\n"), hints];
    if (mode === "regex") return [new RegExp("^(?:" + want + ")$", "s").test(got), hints];
    if (mode === "contains") return [got.includes(want), hints];
    return [got === want, hints];
  }

  const SIGNALS = { 1: "SIGHUP", 2: "SIGINT", 3: "SIGQUIT", 4: "SIGILL", 6: "SIGABRT", 7: "SIGBUS", 8: "SIGFPE", 9: "SIGKILL", 11: "SIGSEGV", 13: "SIGPIPE", 14: "SIGALRM", 15: "SIGTERM", 24: "SIGXCPU", 25: "SIGXFSZ" };

  /** raw: {buildRc, buildLog, progOk, cases: [{out, err, rc, ms}]} (all strings / numbers) -> the object the page shows. */
  function evaluate(ex, raw, opts = {}) {
    const scale = opts.timeScale || 1;
    let log = raw.buildLog.replace(/\/(?:lab|tmp)\/job\/src\//g, "").replace(/\/(?:lab|tmp)\/job\//g, "");
    const built = raw.buildRc === 0 && raw.progOk;
    if (!built) {
      if (raw.buildRc === 0 && ex.build) log += "\nThe build finished but did not create ./prog.";
      return { compiled: false, log, cases: [], passed: false };
    }
    const cases = ex.tests.map((c, i) => {
      const r = raw.cases[i] || { out: "", err: "", rc: -1, ms: 0 };
      let shown;
      if ("cmd" in c) shown = c.cmd.split('"$BIN"').join("./prog").split("$BIN").join("./prog");
      else shown = ["./prog"].concat((c.args || []).map((a) => shq(String(a)))).join(" ");
      if ("stdin" in c) shown += "   < (stdin below)";
      const res = { name: c.name || shown, cmd: shown, stdin: "stdin" in c ? c.stdin : null, hidden: !!c.hidden };
      const limit = caseLimit(c, scale) * 1000;
      if (r.ms >= limit * 0.98 && (r.rc === 137 || r.rc === 124)) {
        return Object.assign(res, { ok: false, got: r.out.slice(0, 2000), stderr: r.err.slice(0, 1000), timeout: true,
          hints: [`timed out after ${c.timeout || 5}s (infinite loop, or waiting for input that never comes?)`] });
      }
      // as in the server: a program run directly (`exec "$@"`) that dies from a signal has a negative status; one run by a `cmd` shell has 128+n
      let code = r.rc;
      if (!("cmd" in c) && code > 128 && code < 160) code = -(code - 128);
      const want = c.stdout || "";
      const mode = c.match || "exact";
      const [okOut, hints] = compare(r.out, want, mode);
      const expExit = "exit" in c ? c.exit : 0;
      let okExit = true;
      if (expExit === "any") okExit = true;
      else if (expExit === "nonzero") okExit = code !== 0;
      else okExit = code === expExit;
      if (!okExit) {
        if (code < 0) hints.push(`the program was killed by signal ${SIGNALS[-code] || -code}` + (-code === 11 ? " (invalid memory access)" : ""));
        else hints.push(`exit status was ${code}, expected ${expExit}`);
      }
      if (c.stderr_empty && r.err.trim()) hints.push("anything that is not the result (errors) should go to stderr, and a correct run prints no errors");
      if (!okOut && !r.out.trim() && r.err.trim()) hints.push("nothing on stdout, but something on stderr: results go to stdout (printf), not stderr");
      const ok = okOut && okExit && !(c.stderr_empty && r.err.trim());
      Object.assign(res, { ok, got: r.out.slice(0, 4000), exit: code, stderr: r.err.slice(0, 1000), hints, match: mode });
      if (!res.hidden) res.expected = mode !== "regex" ? want : `(matches /${want}/)`;
      return res;
    });
    return { compiled: true, log, cases, passed: cases.every((c) => c.ok), build: !!ex.build };
  }

  /** The tar a machine sends back -> raw for evaluate. */
  function readResults(tarBytes, n) {
    const f = Tar.parse(tarBytes);
    const text = (name) => (f[name] ? dec.decode(f[name]) : "");
    const cases = [];
    for (let i = 0; i < n; i++) cases.push({ out: text(`r/${i}.out`), err: text(`r/${i}.err`), rc: f[`r/${i}.rc`] ? parseInt(text(`r/${i}.rc`), 10) : -1, ms: parseInt(text(`r/${i}.ms`) || "0", 10) });
    return { buildLog: text("build.log"), buildRc: parseInt(text("build.rc") || "1", 10), progOk: !!f["r/prog.ok"], cases };
  }

  const api = { makeJob, evaluate, readResults, compare, shq, caseLimit };
  if (typeof module !== "undefined" && module.exports) module.exports = api;
  else root.CellarRunner = api;
})(typeof window !== "undefined" ? window : globalThis);
