#!/usr/bin/env node
// Boots the real machine (v86 + the Alpine image) in Node, with no browser, and runs exercises through it exactly as the page does (src/runner.js).
// Reports: boot time, time per check, and every case that differs from what the reference solution should give. This is the acceptance test of the site.
// Usage: node site/tools/vm_smoke.mjs [--ids a,b,c | --only substring | --all] [--scale 6] [--mem 512] [--hello]
//   --hello   only boots and compiles a program with fork, a pipe and a thread, to see that the machine works at all
// Needs site/dist/vm/ (run site/build_vm.sh first) and site/dist/data (python3 site/build_site.py).
import fs from "node:fs";
import path from "node:path";
import { createRequire } from "node:module";

const require = createRequire(import.meta.url);
const here = path.dirname(new URL(import.meta.url).pathname);
const vmDir = path.join(here, "../dist/vm");
const dataDir = path.join(here, "../dist/data/coding");
const Runner = require("../src/runner.js");
const Tar = require("../src/tar.js");
const Boot = require("../src/boot.js");
const { V86 } = await import(path.join(vmDir, "libv86.mjs"));

const args = process.argv.slice(2);
const opt = (n, d) => { const i = args.indexOf(n); return i < 0 ? d : args[i + 1]; };
const scale = parseFloat(opt("--scale", "6"));
const checkMs = parseFloat(opt("--check-timeout", "300")) * 1000;   // a check that takes longer than this is a hang: it is reported, not waited for
const mem = parseInt(opt("--mem", "512"), 10);
const t0 = Date.now();
const secs = () => ((Date.now() - t0) / 1000).toFixed(1) + "s";

const emu = new V86({
  wasm_path: path.join(vmDir, "v86.wasm"),
  bios: { url: path.join(vmDir, "seabios.bin") },
  vga_bios: { url: path.join(vmDir, "vgabios.bin") },
  bzimage: { url: path.join(vmDir, "bzimage.bin") },
  cmdline: Boot.cmdline,
  filesystem: { baseurl: path.join(vmDir, "rootfs/flat/"), basefs: path.join(vmDir, "rootfs/fs.json") },
  memory_size: mem * 1024 * 1024,
  vga_memory_size: 2 * 1024 * 1024,
  autostart: true,
});

let tail = "";
const waiters = new Set();
const log = args.includes("--serial");
emu.add_listener("serial0-output-byte", (b) => {
  const c = String.fromCharCode(b);
  if (log) process.stdout.write(c);
  tail = (tail + c).slice(-8192);
  for (const w of [...waiters]) if (tail.includes(w.text)) { waiters.delete(w); w.resolve(); }
});
const waitFor = (text, ms, what) => new Promise((resolve, reject) => {
  const w = { text, resolve };
  waiters.add(w);
  setTimeout(() => { if (waiters.delete(w)) reject(new Error(`timeout waiting for ${what || text} after ${ms / 1000}s; last output: ${JSON.stringify(tail.slice(-300))}`)); }, ms);
});
const send = (s) => emu.serial0_send(s);

async function boot() {
  await waitFor(Boot.prompt, 600000, "the kernel's prompt");
  console.log(`kernel up: ${secs()}`);
  if (args.includes("--shell")) {   // type into the kernel's own shell (before entering the Alpine files): ls /mnt, mount, cat /proc/cmdline ... Ctrl-C leaves
    process.stdin.setRawMode(true);
    process.stdin.resume();
    process.stdin.setEncoding("utf8");
    process.stdin.on("data", (c) => { if (c === "\u0003") { emu.destroy(); process.exit(0); } else send(c); });
    await new Promise(() => {});
  }
  const entered = waitFor(Boot.ready, 120000, "entering the Alpine files");
  send(Boot.command + "\n");
  await entered;
  console.log(`booted: ${secs()}`);
  const ready = waitFor("CELLAR-" + "42", 60000, "shell");
  for (let i = 0; i < 20; i++) { send("echo CELLAR-$((40+2))\n"); const ok = await Promise.race([ready.then(() => true), new Promise((r) => setTimeout(() => r(false), 3000))]); if (ok) break; }
  console.log(`shell ready: ${secs()}`);
}

async function check(ex, files) {
  const job = Runner.makeJob(ex, files, { timeScale: scale });
  await emu.create_file(Boot.jobTar, job.tar);
  tail = "";
  const done = waitFor(Boot.jobDone, checkMs, "check " + ex.id);
  send(Boot.job + "\n");
  await done;
  const bytes = await emu.read_file(Boot.resultsTar);
  return Runner.evaluate(ex, Runner.readResults(bytes, job.cases), { timeScale: scale });
}

await boot();

if (args.includes("--hello")) {
  const src = `#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
static void *t(void *a) { (void)a; printf("thread\\n"); return NULL; }
int main(void) {
    int p[2]; char buf[16] = {0}; pthread_t th;
    pipe(p);
    if (fork() == 0) { write(p[1], "child\\n", 6); _exit(0); }
    read(p[0], buf, 15); wait(NULL);
    printf("%s", buf);
    pthread_create(&th, NULL, t, NULL); pthread_join(th, NULL);
    printf("long=%zu ptr=%zu\\n", sizeof(long), sizeof(void *));
    return 0;
}
`;
  const ex = { id: "hello", files: ["answer.c"], headers: {}, hidden: {}, harness: null, build: null, fixtures: { files: {}, dirs: [] },
    tests: [{ args: [], stdout: "child\nthread\nlong=4 ptr=4\n" }] };
  const t1 = Date.now();
  const r = await check(ex, { "answer.c": src });
  console.log(`compile+run: ${((Date.now() - t1) / 1000).toFixed(1)}s`, JSON.stringify(r, null, 1).slice(0, 1500));
  process.exit(r.passed ? 0 : 1);
}

const wanted = opt("--only", args.includes("--all") ? "" : null) || (args.includes("--all") ? "" : null);
const ids = opt("--ids", null) ? opt("--ids", "").split(",") : null;
const shardArg = opt("--shard", null);   // "2/4": the third quarter of the list, so several processes can share the run
let files = fs.readdirSync(dataDir).filter((f) => f.endsWith(".json")).sort();
if (ids) files = files.filter((f) => ids.includes(f.replace(/\.json$/, "")));
else if (wanted !== null) files = files.filter((f) => f.includes(wanted));
else files = files.slice(0, 3);
if (shardArg) { const [i, n] = shardArg.split("/").map(Number); files = files.filter((_, k) => k % n === i); }
const reportFile = path.join(here, "..", shardArg ? `vm_parity_report.${shardArg.replace("/", "of")}.json` : "vm_parity_report.json");
let failed = 0;
const report = [];
for (const f of files) {
  const ex = JSON.parse(fs.readFileSync(path.join(dataDir, f), "utf8"));
  if (ex.site_unavailable) { console.log(`skip ${ex.id} (cannot be checked on the 32-bit machine, by design)`); report.push({ id: ex.id, ok: true, seconds: 0, skipped: true, compiled: true, bad: [], log: "" }); continue; }
  const t1 = Date.now();
  let res;
  try { res = await check(ex, ex.solution_files); } catch (e) { res = { compiled: false, log: String(e.message), cases: [], passed: false }; }
  const dt = (Date.now() - t1) / 1000;
  const bad = res.cases.filter((c) => !c.ok);
  const ok = res.passed;
  if (!ok) failed++;
  report.push({ id: ex.id, ok, seconds: dt, compiled: res.compiled, bad: bad.map((c) => ({ name: c.name, got: c.got.slice(0, 120), hints: c.hints, timeout: !!c.timeout })), log: res.compiled ? "" : res.log.slice(0, 300) });
  console.log(`${ok ? "ok  " : "FAIL"} ${ex.id} ${dt.toFixed(1)}s${ok ? "" : "  " + (res.compiled ? bad.map((c) => c.name + (c.timeout ? " [timeout]" : "")).join("; ") : "does not compile: " + res.log.slice(0, 120))}`);
  fs.writeFileSync(reportFile, JSON.stringify(report, null, 1));   // (after every exercise: stopping the run loses nothing)
}
if (opt("--exec", null)) {   // run a shell command in the machine afterwards (the last job is still in /tmp/job): to find out why something differs
  tail = "";
  const done = waitFor("CELLAR-2END", 300000, "the command");
  send(opt("--exec", "") + "; echo CELLAR-$((1+1))END\n");
  await done;
  console.log(tail.replace(/\r/g, ""));
  process.exit(0);
}
console.log(`${files.length} exercises, ${failed} not passing, total ${secs()}; details in ${path.basename(reportFile)}`);
process.exit(0);
