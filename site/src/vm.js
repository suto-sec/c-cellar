/* The machine of the site: a real Linux (Alpine, 32 bit, gcc) running in the v86 x86 emulator, inside the page.
 * It is started the first time a check or the terminal needs it. Files go in and out through v86's 9p filesystem (create_file / read_file),
 * commands go in through the serial console, which is also what the terminal panel shows.
 *   CellarVM.check(ex, files)   -> the checker result (see runner.js)
 *   CellarVM.compile(d, files, run) / CellarVM.send(text) / CellarVM.onOutput(fn) -> the terminal panel
 * Everything the machine needs is in vm/ (see site/build_vm.sh): libv86.js v86.wasm seabios.bin vgabios.bin bzimage.bin rootfs/ */
(function (root) {
  "use strict";
  const cfg = Object.assign({ dir: "vm/", memoryMB: 512, timeScale: 6, bootTimeoutMs: 180000, checkTimeoutMs: 600000 }, root.CELLAR_VM_CONFIG || {});
  const Runner = root.CellarRunner;
  const Tar = root.CellarTar;
  const dec = new TextDecoder();

  let emu = null, booting = null, tail = "", transcript = [], transcriptSize = 0;
  const waiters = new Set();
  const outputs = new Set();
  const status = new Set();
  let queue = Promise.resolve();

  const say = (msg) => status.forEach((f) => f(msg));

  function loadScript(src) {
    return new Promise((resolve, reject) => {
      const s = document.createElement("script");
      s.src = src;
      s.onload = resolve;
      s.onerror = () => reject(new Error("could not load " + src + ": the machine is not installed on this site"));
      document.head.appendChild(s);
    });
  }

  // serial console: bytes in a buffer, waiters watch it
  let pending = [];
  function onByte(b) {
    pending.push(b);
    if (pending.length === 1) setTimeout(flush, 15);
  }
  function flush() {
    const bytes = new Uint8Array(pending);
    pending = [];
    const text = dec.decode(bytes, { stream: true });
    transcript.push(bytes);
    transcriptSize += bytes.length;
    while (transcriptSize > 300000 && transcript.length > 1) transcriptSize -= transcript.shift().length;
    tail = (tail + text).slice(-8192);
    outputs.forEach((f) => f(bytes));
    for (const w of [...waiters]) if (tail.includes(w.text)) { waiters.delete(w); w.resolve(); }
  }
  function waitFor(text, timeoutMs, what) {
    return new Promise((resolve, reject) => {
      const w = { text, resolve };
      waiters.add(w);
      setTimeout(() => { if (waiters.delete(w)) reject(new Error("the machine did not answer in time" + (what ? " (" + what + ")" : ""))); }, timeoutMs);
    });
  }
  const send = (text) => { if (emu) emu.serial0_send(text); };

  async function boot() {
    if (booting) return booting;
    booting = (async () => {
      say("Loading the emulator…");
      await loadScript(cfg.dir + "libv86.js");
      say("Starting the machine (the first time it downloads about 60 MB, which the browser then keeps)…");
      emu = new root.V86({
        wasm_path: cfg.dir + "v86.wasm",
        bios: { url: cfg.dir + "seabios.bin" },
        vga_bios: { url: cfg.dir + "vgabios.bin" },
        bzimage: { url: cfg.dir + "bzimage.bin" },
        cmdline: root.CellarBoot.cmdline,
        filesystem: { baseurl: cfg.dir + "rootfs/flat/", basefs: cfg.dir + "rootfs/fs.json" },
        memory_size: cfg.memoryMB * 1024 * 1024,
        vga_memory_size: 2 * 1024 * 1024,
        autostart: true,
      });
      emu.add_listener("serial0-output-byte", onByte);
      await waitFor(root.CellarBoot.prompt, cfg.bootTimeoutMs, "boot");   // the kernel's own prompt: now go into the Alpine files
      const entered = waitFor(root.CellarBoot.ready, cfg.bootTimeoutMs, "entering the Alpine files");
      send(root.CellarBoot.command + "\n");
      await entered;
      // wait for the shell: typing a sum, the answer cannot be in the echo of the question
      const ready = waitFor("CELLAR-" + "42", cfg.bootTimeoutMs, "shell");
      for (let i = 0; i < 20; i++) { send("echo CELLAR-$((40+2))\n"); const ok = await Promise.race([ready.then(() => true), new Promise((r) => setTimeout(() => r(false), 3000))]); if (ok) break; }
      say("");
    })();
    booting.catch(() => { booting = null; });
    return booting;
  }

  function enqueue(fn) {   // one job at a time
    const run = queue.then(fn, fn);
    queue = run.catch(() => {});
    return run;
  }

  /** Throw the machine away (a hang, or a program that ignores Ctrl-C): the next use starts a new one. */
  function restart() {
    try { if (emu) emu.destroy(); } catch (e) { /* already gone */ }
    emu = null; booting = null; tail = ""; pending = [];
    waiters.clear();
    say("Restarting the machine…");
  }

  /** Make sure the shell is at its prompt: a program the student left running in the terminal would eat the next command. */
  async function ensureShell() {
    for (let attempt = 0; attempt < 2; attempt++) {
      tail = "";
      const ok = waitFor("CELLAR-7SH", 6000, "shell").then(() => true, () => false);
      send("\x03\x03\n");
      setTimeout(() => send("echo CELLAR-$((3+4))SH\n"), 300);
      if (await ok) return;
    }
    restart();
    await boot();
  }

  async function check(ex, files) {
    return enqueue(async () => {
      await boot();
      await ensureShell();
      const job = Runner.makeJob(ex, files, { timeScale: cfg.timeScale });
      await emu.create_file(root.CellarBoot.jobTar, job.tar);
      tail = "";   // (a marker left by an earlier job must not count)
      // the job's own limits, plus time to build: a check that takes longer than that is a hang
      const limit = 120000 + ex.tests.reduce((a, c) => a + Runner.caseLimit(c, cfg.timeScale) * 1000, 0);
      const done = waitFor(root.CellarBoot.jobDone, limit, "check");
      send(root.CellarBoot.job + "\n");
      try { await done; }
      catch (e) { restart(); throw new Error("The machine did not answer in time and was restarted. Press Check again."); }
      const bytes = await emu.read_file(root.CellarBoot.resultsTar);
      return Runner.evaluate(ex, Runner.readResults(bytes, job.cases), { timeScale: cfg.timeScale });
    });
  }

  /** Compile (and run) in the terminal: the sources go to /lab/work.tar and the helper lab-compile unpacks and builds them in front of the student. */
  async function compile(ex, files, run) {
    return enqueue(async () => {
      await boot();
      await ensureShell();
      const entries = ex.files.map((n) => ({ name: n, data: files[n] || "" }));
      for (const [n, t] of Object.entries(ex.headers || {})) entries.push({ name: n, data: t });
      for (const [n, t] of Object.entries(ex.hidden || {})) entries.push({ name: n, data: t });
      if (ex.harness) entries.push({ name: "harness.c", data: ex.harness });
      await emu.create_file("/lab/work.tar", Tar.create(entries));
      send(`lab-compile ${ex.id} ${Runner.shq(ex.compile_cmd)}${run ? " run" : ""}\n`);
    });
  }

  const mock = !!root.CELLAR_MOCK_VM || (typeof location !== "undefined" && /[?&]mockvm\b/.test(location.search));   // for the browser tests: answers at once, no machine
  const mockCheck = async (ex) => ({ compiled: true, log: "", passed: true, build: false,
    cases: ex.tests.slice(0, 2).map((c, i) => ({ name: "./prog case " + (i + 1), cmd: "./prog", stdin: null, hidden: false, ok: true, got: "", exit: 0, stderr: "", hints: [], match: "exact", expected: "" })) });
  root.CellarVM = {
    boot: mock ? async () => {} : boot, check: mock ? mockCheck : check, compile: mock ? async () => {} : compile, send,
    onOutput(fn) { outputs.add(fn); return () => outputs.delete(fn); },
    onStatus(fn) { status.add(fn); return () => status.delete(fn); },
    replay(fn) { for (const b of transcript) fn(b); },
    booted: () => !!emu && !!tail,
    config: cfg,
  };
})(typeof window !== "undefined" ? window : globalThis);

/* ?autotest=<exercise id>: boots the machine, checks the reference solution of that exercise and writes the outcome in <pre id="cellar-autotest">
 * (to see the whole thing work in a real browser without typing: open  index.html?autotest=t3-output-hello-world  from a web server). */
(function (root) {
  const m = typeof location !== "undefined" && /[?&]autotest=([\w-]+)/.exec(location.search);
  if (!m) return;
  root.addEventListener("load", async () => {
    const out = document.createElement("pre");
    out.id = "cellar-autotest";
    out.textContent = "running…";
    document.body.appendChild(out);
    try {
      const ex = await (await fetch(`data/coding/${m[1]}.json`)).json();
      const t = Date.now();
      const r = await root.CellarVM.check(ex, ex.solution_files);
      out.textContent = JSON.stringify({ id: ex.id, passed: r.passed, compiled: r.compiled, seconds: (Date.now() - t) / 1000, failing: r.cases.filter((c) => !c.ok).map((c) => c.name), log: r.log });
    } catch (e) { out.textContent = "ERROR " + e.message; }
  });
})(typeof window !== "undefined" ? window : globalThis);
