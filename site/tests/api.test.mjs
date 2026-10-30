// src/api.js against the real server: same index and exercise data (with no progress), and the progress flow (check -> passed, skip, theory answer).
import fs from "node:fs";
import os from "node:os";
import path from "node:path";
import { createRequire } from "node:module";
import { execFileSync, spawnSync } from "node:child_process";
const require = createRequire(import.meta.url);
const here = path.dirname(new URL(import.meta.url).pathname);
const dist = path.join(here, "../dist");
const Runner = require("../src/runner.js");
const serverIndex = JSON.parse(fs.readFileSync(process.argv[2], "utf8"));
const serverCoding = JSON.parse(fs.readFileSync(process.argv[3], "utf8"));   // {id: response}

globalThis.fetch = async (p) => {
  const f = path.join(dist, p);
  return fs.existsSync(f) ? { ok: true, json: async () => JSON.parse(fs.readFileSync(f, "utf8")) } : { ok: false, status: 404 };
};
// a machine made of this computer's bash, as in verify_native.mjs
globalThis.CellarRunner = Runner;
globalThis.CellarVM = {
  async check(ex, files) {
    const job = Runner.makeJob(ex, files);
    const dir = fs.mkdtempSync(path.join(process.env.TMPDIR || os.tmpdir(), "apitest-"));
    fs.writeFileSync(path.join(dir, "job.tar"), job.tar);
    execFileSync("tar", ["-xf", "job.tar"], { cwd: dir });
    spawnSync("bash", ["ctl/run.sh"], { cwd: dir, env: { ...process.env, J: dir }, stdio: "ignore" });
    const res = Runner.evaluate(ex, Runner.readResults(new Uint8Array(fs.readFileSync(path.join(dir, "results.tar"))), job.cases));
    fs.rmSync(dir, { recursive: true, force: true });
    return res;
  },
};
const store = new Map();
globalThis.localStorage = { getItem: (k) => (store.has(k) ? store.get(k) : null), setItem: (k, v) => store.set(k, String(v)), removeItem: (k) => store.delete(k), get length() { return store.size; }, key: (i) => [...store.keys()][i] };
require("../src/api.js");
const api = globalThis.CELLAR_API;
let bad = 0;
const eq = (name, a, b) => { if (JSON.stringify(a) !== JSON.stringify(b)) { bad++; console.log("DIFF", name, JSON.stringify(a).slice(0, 200), "|", JSON.stringify(b).slice(0, 200)); } };

// 1. the index of a student with no progress is the server's
const idx = await api("index");
for (const t of serverIndex.theory) for (const q of t.qs) delete q.s_unused;
const strip = (x) => JSON.parse(JSON.stringify(x));
eq("index.coding", idx.coding, serverIndex.coding);
eq("index.theory (ids aside)", idx.theory, serverIndex.theory);
eq("index.chapters", idx.chapters, serverIndex.chapters);
eq("index.learning_paths", idx.learning_paths, serverIndex.learning_paths);
eq("index.paths", idx.paths, serverIndex.paths);
eq("index.reference", idx.reference, serverIndex.reference);

// 2. the exercise pages
let n = 0;
for (const [id, s] of Object.entries(serverCoding)) {
  const d = await api("coding/" + id);
  if (d.statement !== s.statement && !d.statement.startsWith(s.statement.trimEnd())) eq(`coding/${id}.statement`, d.statement.slice(0, 80), s.statement.slice(0, 80));   // (the site adds a note to the exercises it cannot grade)
  for (const k of ["id", "tag", "title", "track", "topic", "stars", "minutes", "hints", "recommended", "files", "theory", "status", "skipped"]) eq(`coding/${id}.${k}`, d[k], s[k]);
  if (!/harness/.test(s.compile_cmd)) eq(`coding/${id}.compile_cmd`, d.compile_cmd, s.compile_cmd);
  n++;
}
console.log(`${n} exercise pages compared`);

// 3. progress: nothing written -> "is empty"; the reference solution -> passed; skip; solution; theory answer
let r = await api("coding/t3-output-hello-world/check", { date: "2026-10-30" });
eq("empty answer", [r.compiled, r.passed, r.log], [false, false, "answer.c is empty: write your program there first."]);
const sol = JSON.parse(fs.readFileSync(path.join(dist, "data/coding/t3-output-hello-world.json"), "utf8")).solution_files["answer.c"];
localStorage.setItem("cellar.site.code.v1.t3-output-hello-world/answer.c", sol);
r = await api("coding/t3-output-hello-world/check", { date: "2026-10-30" });
eq("passes", [r.compiled, r.passed, r.status], [true, true, "passed"]);
localStorage.setItem("cellar.site.code.v1.t3-output-hello-world/answer.c", "int main(void){return 0;}");
r = await api("coding/t3-output-hello-world/check", { date: "2026-10-30" });
eq("wrong program fails but stays passed", [r.passed, r.status], [false, "passed"]);
let s2 = await api("coding/t3-output-numbers/solution", {});
eq("solution marks it", s2.status, "solution");
await api("coding/t3-output-numbers/skip", { on: true });
let i2 = await api("index");
eq("index overlay", i2.coding.filter((e) => ["t3-output-hello-world", "t3-output-numbers"].includes(e.id)).map((e) => [e.id, e.status, e.skipped]).sort(),
  [["t3-output-hello-world", "passed", false], ["t3-output-numbers", "solution", true]]);
const set = await api("theory/t3-th-output");
const q = set.questions[0];
const a = await api("theory/t3-th-output/answer", { id: q.id, response: q.type === "single" ? 999 : [] });
eq("wrong answer reveals", [a.correct, "explain" in a], [false, true]);
i2 = await api("index");
const row = i2.theory.find((t) => t.id === "t3-th-output");
eq("theory counts", [row.answered, row.correct], [1, 0]);
eq("favorites", (await api("reference/favorite", { id: "t3-ref-printf", on: true })).favorites, ["t3-ref-printf"]);
await api("progress/reset", {});
eq("reset", (await api("index")).coding.every((e) => e.status === "todo" && !e.skipped), true);
console.log(bad ? `${bad} difference(s)` : "api.js agrees with the server");
process.exit(bad ? 1 : 0);
