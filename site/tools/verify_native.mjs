#!/usr/bin/env node
// Runs every exercise's reference solution through the SAME job the browser sends to its VM (src/runner.js), but with this machine's own
// bash and gcc instead of the VM. If this passes, the job scripts and the result evaluation are right; what is left to prove is the VM itself.
// Usage: node site/tools/verify_native.mjs [--jobs N] [--only substring] [--mutate]   (needs site/dist/data from build_site.py)
import fs from "node:fs";
import os from "node:os";
import path from "node:path";
import { execFileSync, spawn } from "node:child_process";
import { createRequire } from "node:module";

const require = createRequire(import.meta.url);
const here = path.dirname(new URL(import.meta.url).pathname);
const Runner = require(path.join(here, "../src/runner.js"));
const Tar = require(path.join(here, "../src/tar.js"));

const args = process.argv.slice(2);
const opt = (name, def) => { const i = args.indexOf(name); return i < 0 ? def : args[i + 1]; };
const jobs = parseInt(opt("--jobs", "3"), 10);
const only = opt("--only", "");
const mutate = args.includes("--mutate");
const dataDir = path.join(here, "../dist/data/coding");
const tmpRoot = fs.mkdtempSync(path.join(process.env.TMPDIR || os.tmpdir(), "native-"));

function runNative(job, id) {
  const dir = path.join(tmpRoot, id);
  fs.mkdirSync(dir, { recursive: true });
  fs.writeFileSync(path.join(dir, "job.tar"), job.tar);
  execFileSync("tar", ["-xf", "job.tar"], { cwd: dir });
  return new Promise((resolve) => {
    const p = spawn("bash", ["ctl/run.sh"], { cwd: dir, env: { ...process.env, J: dir }, stdio: "ignore" });
    p.on("close", () => {
      const f = path.join(dir, "results.tar");
      resolve(fs.existsSync(f) ? new Uint8Array(fs.readFileSync(f)) : null);
    });
  });
}

const files = fs.readdirSync(dataDir).filter((f) => f.endsWith(".json") && f.includes(only)).sort();
let next = 0, failed = 0, done = 0;
const problems = [];

async function one(file) {
  const ex = JSON.parse(fs.readFileSync(path.join(dataDir, file), "utf8"));
  let sources = ex.solution_files;
  // (an "empty" answer: a do-nothing main in the .c files, and nothing in the others: a Makefile, a header)
  if (mutate) sources = Object.fromEntries(Object.keys(sources).map((n) => [n, n.endsWith(".c") ? "int main(void) { return 0; }\n" : ""]));
  const job = Runner.makeJob(ex, sources);
  const tarBytes = await runNative(job, ex.id);
  if (!tarBytes) { problems.push(`${ex.id}: no results.tar`); failed++; return; }
  const res = Runner.evaluate(ex, Runner.readResults(tarBytes, job.cases));
  if (mutate) {
    if (res.passed) { problems.push(`${ex.id}: an empty program PASSED`); failed++; }
    return;
  }
  const bad = res.cases.filter((c) => !c.ok);
  if (!res.compiled) { problems.push(`${ex.id}: does not compile\n${res.log.slice(0, 300)}`); failed++; }
  else if (bad.length) { problems.push(`${ex.id}: ${bad.length}/${res.cases.length} cases fail: ${bad.map((c) => c.name + " -> " + JSON.stringify(c.got.slice(0, 60)) + " " + (c.hints || []).join("; ")).join(" | ").slice(0, 400)}`); failed++; }
  else if (res.log.trim() && !ex.build) { problems.push(`${ex.id}: compiles with output\n${res.log.slice(0, 200)}`); failed++; }
}

async function worker() {
  while (next < files.length) {
    const f = files[next++];
    await one(f);
    if (++done % 50 === 0) console.error(`  ${done}/${files.length}`);
  }
}

await Promise.all(Array.from({ length: jobs }, worker));
fs.rmSync(tmpRoot, { recursive: true, force: true });
console.log(`${files.length} exercises, ${failed} problem(s)${mutate ? " (mutated: every empty program must fail)" : ""}`);
for (const p of problems) console.log(" -", p);
process.exit(failed ? 1 : 0);
