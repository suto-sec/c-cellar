// The page's own code path to the machine, in Node: src/{tar,runner,api,boot,vm}.js are loaded the way index.html loads them (as scripts, into a
// "window"), with just enough of a browser around them (document.createElement("script") loads a file, localStorage, fetch of data/ files).
// Then the exercise API is called like app.js calls it: write code into the editor's store, press Check (api "coding/<id>/check").
// Usage: node site/tests/page_vm.test.mjs [exercise id ...]   (needs dist/ with data and vm/)
import fs from "node:fs";
import path from "node:path";
import vm from "node:vm";

const here = path.dirname(new URL(import.meta.url).pathname);
const dist = path.join(here, "../dist");
const src = path.join(here, "../src");

const store = new Map();
const win = globalThis;
win.window = win;
win.localStorage = { getItem: (k) => (store.has(k) ? store.get(k) : null), setItem: (k, v) => store.set(k, String(v)), removeItem: (k) => store.delete(k), get length() { return store.size; }, key: (i) => [...store.keys()][i] };
win.location = { search: "", href: "http://localhost/" };
win.CELLAR_VM_CONFIG = { dir: path.join(dist, "vm") + "/" };
const realFetch = globalThis.fetch;
win.fetch = async (p) => {   // the site's data files are read from dist/
  const f = path.join(dist, String(p));
  return fs.existsSync(f) ? { ok: true, json: async () => JSON.parse(fs.readFileSync(f, "utf8")) } : { ok: false, status: 404 };
};
win.document = {
  head: { appendChild(el) {   // vm.js loads libv86.js with a <script>: run it and call onload
    try { vm.runInThisContext(fs.readFileSync(el.src, "utf8"), { filename: el.src, importModuleDynamically: vm.constants.USE_MAIN_CONTEXT_DEFAULT_LOADER }); setTimeout(() => el.onload && el.onload(), 0); }
    catch (e) { console.error("script failed:", e); setTimeout(() => el.onerror && el.onerror(e), 0); }
  } },
  createElement: () => ({}),
  body: { appendChild() {} },
};
win.addEventListener = () => {};
win.removeEventListener = () => {};   // (the browser build of v86 sets up keyboard and mouse listeners on window and document)
win.document.addEventListener = () => {};
win.document.removeEventListener = () => {};
for (const f of ["tar.js", "runner.js", "api.js", "boot.js", "vm.js"]) vm.runInThisContext(fs.readFileSync(path.join(src, f), "utf8"), { filename: f });
if (typeof win.V86 !== "function") console.log("(V86 is not defined until the machine is started)");

const ids = process.argv.slice(2).length ? process.argv.slice(2) : ["t3-output-hello-world", "t4-procs-fork-hello", "t3-build-header-and-source"];
let bad = 0;
const t0 = Date.now();
for (const id of ids) {
  const data = JSON.parse(fs.readFileSync(path.join(dist, "data/coding", id + ".json"), "utf8"));
  if (data.site_unavailable) {   // an exercise the 32-bit machine cannot grade: Check says so, counts no attempt, and does not touch the status
    for (const n of data.files) win.CellarStore.setCode(id, n, data.solution_files[n] || "");
    const r = await win.CELLAR_API(`coding/${id}/check`, { date: "2026-10-30" });
    const idx0 = (await win.CELLAR_API("index")).coding.find((e) => e.id === id);
    const okU = !!r.unavailable && !r.passed && idx0.status === "todo" && data.statement.includes("Browser edition:");
    if (!okU) bad++;
    console.log(`${okU ? "ok  " : "FAIL"} ${id}: cannot be checked here -> "${(r.unavailable || "").slice(0, 50)}…", status ${idx0.status}`);
    continue;
  }
  // 1. a wrong program: must compile or not, and fail
  for (const n of data.files) win.CellarStore.setCode(id, n, n.endsWith(".c") ? "int main(void) { return 0; }\n" : "");
  const wrong = await win.CELLAR_API(`coding/${id}/check`, { date: "2026-10-30" });
  // 2. the reference solution: must pass
  for (const n of data.files) win.CellarStore.setCode(id, n, data.solution_files[n] || "");
  const right = await win.CELLAR_API(`coding/${id}/check`, { date: "2026-10-30" });
  const idx = (await win.CELLAR_API("index")).coding.find((e) => e.id === id);
  const ok = !wrong.passed && right.passed && right.status === "passed" && idx.status === "passed";
  if (!ok) bad++;
  console.log(`${ok ? "ok  " : "FAIL"} ${id}: wrong program -> ${wrong.compiled ? (wrong.passed ? "PASSED?!" : "fails") : "does not compile"}; reference -> ${right.passed ? "passes" : "FAILS " + JSON.stringify(right.cases.filter((c) => !c.ok).map((c) => c.name)) + right.log.slice(0, 200)}; status ${idx.status} (${((Date.now() - t0) / 1000).toFixed(0)}s)`);
}
// 3. Compile & run goes to the terminal: the sources arrive in /lab/work.tar and lab-compile builds them
const hello = JSON.parse(fs.readFileSync(path.join(dist, "data/coding/t3-output-hello-world.json"), "utf8"));
let out = "";
const off = win.CellarVM.onOutput((b) => { out += Buffer.from(b).toString(); });
await win.CellarVM.compile(hello, { "answer.c": hello.solution_files["answer.c"] }, true);
const t1 = Date.now();
while (!/exit status \d+\]/.test(out) && Date.now() - t1 < 120000) await new Promise((r) => setTimeout(r, 500));
off();
const ran = /Hello, world!\r?\n\[exit status 0\]/.test(out);
if (!ran) bad++;
console.log(`${ran ? "ok  " : "FAIL"} Compile & run in the terminal: ${JSON.stringify(out.replace(/\x1b\[[0-9;?]*[ -/]*[@-~]/g, "").slice(-300))}`);
console.log(bad ? `${bad} problem(s)` : "the page's machine code path works");
process.exit(bad ? 1 : 0);
void realFetch;
