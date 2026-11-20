/* The site's replacement for the server: window.CELLAR_API(path, body) answers every call app.js makes to /api/ from the data bundle
 * (data/*.json) and the student's progress in localStorage. The theory grading is a port of app/server.py (grade, blanks_ok, pairs_ok...).
 * Code that needs the machine (checking an exercise) goes through window.CellarVM (vm.js). */
(function (root) {
  "use strict";
  const PROGRESS_KEY = "cellar.site.progress.v1";
  const CODE_KEY = "cellar.site.code.v1.";
  const PATHS_KEY = "cellar.site.paths.v1";   // the paths the student imported: a list of the files as they were written
  const mem = { progress: null, code: {}, paths: null };   // used when localStorage is not available (a private window...)

  // ------------------------------------------------------------------ storage
  function ls() { try { return root.localStorage; } catch (e) { return null; } }
  function loadProgress() {
    if (mem.progress) return mem.progress;
    let p = null;
    try { p = JSON.parse(ls().getItem(PROGRESS_KEY)); } catch (e) { p = null; }
    if (!p || typeof p !== "object") p = {};
    p.coding = p.coding || {}; p.theory = p.theory || {}; p.favorites = p.favorites || [];
    mem.progress = p;
    return p;
  }
  function saveProgress() {
    try { ls().setItem(PROGRESS_KEY, JSON.stringify(mem.progress)); } catch (e) { /* kept in memory only */ }
  }
  function loadPaths() {
    if (mem.paths) return mem.paths;
    let l = null;
    try { l = JSON.parse(ls().getItem(PATHS_KEY)); } catch (e) { l = null; }
    mem.paths = Array.isArray(l) ? l.filter((p) => p && typeof p === "object") : [];
    return mem.paths;
  }
  function savePaths() {
    try { ls().setItem(PATHS_KEY, JSON.stringify(mem.paths)); } catch (e) { /* kept in memory only */ }
  }
  const store = {
    getCode(id, name) {
      const k = CODE_KEY + id + "/" + name;
      try { const v = ls().getItem(k); if (v !== null) return v; } catch (e) { /* memory */ }
      return mem.code[k] || "";
    },
    setCode(id, name, text) {
      const k = CODE_KEY + id + "/" + name;
      mem.code[k] = text;
      try { ls().setItem(k, text); } catch (e) { /* memory only */ }
    },
    clearCode(id, names) {
      for (const n of names) {
        const k = CODE_KEY + id + "/" + n;
        delete mem.code[k];
        try { ls().removeItem(k); } catch (e) { /* nothing */ }
      }
    },
    exportAll() {
      const out = { progress: loadProgress(), code: {}, paths: loadPaths() };
      try { for (let i = 0; i < ls().length; i++) { const k = ls().key(i); if (k.startsWith(CODE_KEY)) out.code[k] = ls().getItem(k); } } catch (e) { /* nothing */ }
      return out;
    },
    importAll(data) {
      mem.progress = data.progress; saveProgress();
      if (Array.isArray(data.paths)) { mem.paths = data.paths; savePaths(); }
      for (const [k, v] of Object.entries(data.code || {})) { try { ls().setItem(k, v); } catch (e) { mem.code[k] = v; } }
    },
  };

  // ------------------------------------------------------------------ data
  const cache = new Map();
  function getJson(path) {
    if (root.CELLAR_DATA && root.CELLAR_DATA[path]) return Promise.resolve(root.CELLAR_DATA[path]);   // the file:// test build: all the data is in one script
    if (!cache.has(path)) {
      cache.set(path, fetch(path).then((r) => {
        if (!r.ok) throw new Error("not found: " + path);
        return r.json();
      }).catch((e) => { cache.delete(path); throw e; }));
    }
    return cache.get(path);
  }
  const clone = (x) => JSON.parse(JSON.stringify(x));

  function codingStatus(e) {   // app/server.py coding_status
    if (!e) return "todo";
    if (e.passed) return "passed";
    if (e.solution_viewed) return "solution";
    if (e.attempts) return "progress";
    return "todo";
  }

  // ------------------------------------------------------------------ theory grading (port of app/server.py)
  function norm(s, ignoreSpace, kase) {
    s = String(s).trim().replace(/\s+/g, ignoreSpace ? "" : " ");
    return kase ? s : s.toLowerCase();
  }
  function blanksOk(q, response) {
    if (!Array.isArray(response) || response.length !== q.blanks.length) return q.blanks.map(() => false);
    return response.map((given, i) => {
      const blank = q.blanks[i];
      const accepted = Array.isArray(blank) ? blank : blank.accept;
      const ign = !Array.isArray(blank) && !!blank.ignore_space, kase = !Array.isArray(blank) && !!blank.case;
      const g = norm(given, ign, kase);
      return accepted.some((a) => {
        if (a.startsWith("re:")) return new RegExp("^(?:" + a.slice(3) + ")$", kase ? "" : "i").test(String(given).trim());
        return g === norm(a, ign, kase);
      });
    });
  }
  function outputLines(text) {
    const lines = String(text).replace(/\r/g, "").split("\n").map((l) => l.replace(/\s+$/, ""));
    while (lines.length && !lines[lines.length - 1]) lines.pop();
    return lines;
  }
  const answersOf = (q) => (Array.isArray(q.answer) ? q.answer : [q.answer]);
  function pairsOk(q, response) {
    const right = (q.type === "match" ? q.pairs : q.items).map((p) => p[1]);
    if (!Array.isArray(response) || response.length !== right.length) return right.map(() => false);
    return response.map((g, i) => g === right[i]);
  }
  function grade(q, response) {
    const t = q.type;
    if (t === "single") return response === q.answer;
    if (t === "multiple") return Array.isArray(response) && JSON.stringify([...response].sort((a, b) => a - b)) === JSON.stringify([...q.answer].sort((a, b) => a - b));
    if (t === "order") return JSON.stringify(response) === JSON.stringify(q.items.map((_, i) => i));
    if (t === "fill") return blanksOk(q, response).every(Boolean);
    if (t === "predict") return answersOf(q).some((a) => JSON.stringify(outputLines(a)) === JSON.stringify(outputLines(response)));
    if (t === "match" || t === "sort") return pairsOk(q, response).every(Boolean);
    return false;
  }
  function predictHints(q, response) {
    const got = String(response), want = answersOf(q)[0];
    const words = (t) => t.split(/\s+/).filter(Boolean);
    if (!got.trim()) return ["type what the program prints, exactly (several lines are allowed)"];
    if (words(got.toLowerCase()).join(" ") === words(want.toLowerCase()).join(" ") && words(got).join(" ") !== words(want).join(" ")) return ["right text, but upper/lower case differs"];
    if (words(got).join(" ") === words(want).join(" ")) return ["same words, but the spacing or the line breaks differ"];
    if (outputLines(got).length !== outputLines(want).length) return [`the program prints ${outputLines(want).length} line(s); you typed ${outputLines(got).length}`];
    return [];
  }
  function shuffled(a) {
    const b = a.slice();
    for (let i = b.length - 1; i > 0; i--) { const j = Math.floor(Math.random() * (i + 1)); [b[i], b[j]] = [b[j], b[i]]; }
    return b;
  }
  function publicQuestion(q) {
    const out = {};
    for (const k of ["id", "type", "topic", "tag", "prompt", "code", "difficulty"]) if (k in q) out[k] = q[k];
    if (q.type === "single" || q.type === "multiple") out.options = q.options;
    else if (q.type === "order") {
      let idx = q.items.map((_, i) => i);
      for (let n = 0; n < 20; n++) { idx = shuffled(idx); if (idx.some((v, i) => v !== i) || idx.length < 2) break; }
      out.items = idx.map((i) => ({ id: i, text: q.items[i] }));
    } else if (q.type === "fill") out.blanks = q.blanks.length;
    else if (q.type === "match") { out.items = q.pairs.map((p) => p[0]); out.options = shuffled(q.pairs.map((p) => p[1])); }
    else if (q.type === "sort") { out.items = q.items.map((it) => it[0]); out.categories = q.categories; }
    return out;
  }
  function answerReveal(q) {
    const out = { explain: q.explain || "" };
    const t = q.type;
    if (t === "single" || t === "multiple") { out.answer = q.answer; out.option_notes = q.option_notes || null; }
    else if (t === "order") out.answer = q.items;
    else if (t === "fill") out.answer = q.blanks.map((b) => (Array.isArray(b) ? b : b.accept)[0].replace(/^re:/, ""));
    else if (t === "predict") out.answer = answersOf(q)[0];
    else if (t === "match") out.answer = q.pairs.map((p) => p[1]);
    else if (t === "sort") out.answer = q.items.map((it) => it[1]);
    return out;
  }

  // ------------------------------------------------------------------ imported paths (port of normalize_custom_path / load_custom_paths in app/server.py)
  const SLUG = /^[a-z0-9][a-z0-9-]{0,39}$/;
  const isText = (x) => typeof x === "string";
  const isObject = (x) => x !== null && typeof x === "object" && !Array.isArray(x);
  /** Checks a path file and returns {path, warnings}; throws an Error with a message for the student when it cannot be used. */
  function normalizeCustomPath(raw, codingIds, chapterKeys, taken) {
    if (!isObject(raw)) throw new Error('a path file is a JSON object ({"id": ..., "title": ..., "phases": [...]})');
    const pid = raw.id;
    if (!isText(pid) || !SLUG.test(pid)) throw new Error("`id` must be 1-40 characters: lowercase letters, digits and hyphens, not starting with a hyphen");
    if (taken.has(pid)) throw new Error(`\`id\` "${pid}" is the id of a built-in path: choose another`);
    const title = raw.title;
    if (!isText(title) || !title.trim() || title.length > 80) throw new Error("`title` must be a text of 1-80 characters");
    const blurb = "blurb" in raw ? raw.blurb : "";
    if (!isText(blurb) || blurb.length > 600) throw new Error("`blurb` must be a text of at most 600 characters");
    if (("phases" in raw) === ("items" in raw)) throw new Error("give either `items` (a list of exercise ids) or `phases` (a list of {title, note, items}), not both and not neither");
    const phasesIn = "phases" in raw ? raw.phases : [{ title, items: raw.items }];
    if (!Array.isArray(phasesIn) || phasesIn.length < 1 || phasesIn.length > 40) throw new Error("`phases` must be a list of 1-40 phases");
    const warnings = [], seen = new Set(), phases = [];
    phasesIn.forEach((ph, k) => {
      const n = k + 1;
      if (!isObject(ph) || !Array.isArray(ph.items)) throw new Error(`phase ${n}: it must be an object with a list \`items\``);
      const ptitle = "title" in ph ? ph.title : `Part ${n}`, note = "note" in ph ? ph.note : "";
      if (!isText(ptitle) || !ptitle.trim() || ptitle.length > 80) throw new Error(`phase ${n}: \`title\` must be a text of 1-80 characters`);
      if (!isText(note) || note.length > 6000) throw new Error(`phase ${n}: \`note\` must be a text of at most 6000 characters`);
      const items = [];
      for (const i of ph.items) {
        if (!isText(i)) throw new Error(`phase ${n}: every item must be an exercise id (a text)`);
        if (!codingIds.has(i)) warnings.push(`phase ${n}: there is no exercise "${i}" here: left out`);
        else if (seen.has(i)) warnings.push(`phase ${n}: "${i}" is already in the path: kept only the first time`);
        else { seen.add(i); items.push(i); }
      }
      const theory = "theory" in ph ? ph.theory : [];
      if (!Array.isArray(theory) || !theory.every(isText)) throw new Error(`phase ${n}: \`theory\` must be a list of chapters such as "t4/signals"`);
      for (const c of theory) if (!chapterKeys.has(c)) warnings.push(`phase ${n}: there is no chapter "${c}": left out`);
      phases.push({ title: ptitle.trim(), note: note.trim(), items, theory: theory.filter((c) => chapterKeys.has(c)) });
    });
    const total = phases.reduce((a, p) => a + p.items.length, 0);
    if (!total) throw new Error("none of the exercises of the file exists in this lab, so there is nothing to study");
    if (total > 600) throw new Error("a path may have at most 600 exercises");
    return { path: { id: pid, title: title.trim(), blurb: blurb.trim(), phases: "phases" in raw ? phases : [], items: phases.flatMap((p) => p.items) }, warnings };
  }
  const idsOf = (idx) => ({ coding: new Set(idx.coding.map((c) => c.id)), chapters: new Set(idx.chapters.map((c) => c.tag + "/" + c.id)),
    builtin: new Set(["course", ...Object.values(idx.learning_paths || {}).flat().map((p) => p.id)]) });
  function customPaths(idx) {
    const k = idsOf(idx), out = [];
    for (const raw of loadPaths()) {
      try { const { path } = normalizeCustomPath(raw, k.coding, k.chapters, new Set()); path.theory = path.phases.flatMap((p) => p.theory); path.source = raw; out.push(path); } catch (e) { /* no longer fits: not shown */ }
    }
    return out;
  }

  // ------------------------------------------------------------------ routes
  const notHere = () => { throw new Error("this part is only in the lab version (daily challenge and readiness need the server)"); };

  async function api(path, body) {
    const [route, query] = path.split("?");
    const parts = route.split("/");
    const prog = loadProgress();
    const post = body !== undefined;

    if (route === "config") return getJson("data/config.json");
    if (route === "index") {
      const idx = clone(await getJson("data/index.json"));
      for (const e of idx.coding) { const p = prog.coding[e.id]; e.status = codingStatus(p); e.skipped = !!(p && p.skipped); }
      for (const s of idx.theory) {
        let answered = 0, correct = 0;
        for (const q of s.qs) {
          const p = prog.theory[q.id];
          q.a = !!p; q.c = !!(p && p.last_correct);
          if (q.a) answered++;
          if (q.c) correct++;
          delete q.id;
        }
        s.answered = answered; s.correct = correct;
      }
      idx.custom_paths = customPaths(idx);
      return idx;
    }
    if (route === "paths/import" && post) {
      const idx = await getJson("data/index.json"), k = idsOf(idx);
      const raw = body.path !== undefined ? body.path : body;
      const { path, warnings } = normalizeCustomPath(raw, k.coding, k.chapters, k.builtin);
      const list = loadPaths(), at = list.findIndex((p) => p && p.id === path.id);
      if (at >= 0) list[at] = raw; else list.push(raw);
      savePaths();
      return { ok: true, id: path.id, title: path.title, count: path.items.length, replaced: at >= 0, warnings };
    }
    if (route === "paths/delete" && post) {
      const list = loadPaths(), at = list.findIndex((p) => p && p.id === body.id);
      if (at < 0) throw new Error("there is no imported path with that id");
      list.splice(at, 1); savePaths();
      return { ok: true };
    }
    if (parts[0] === "coding" && parts.length === 2 && !post) {
      const ex = clone(await getJson(`data/coding/${parts[1]}.json`));
      const p = prog.coding[ex.id] || {};
      for (const k of ["tests", "fixtures", "headers", "hidden", "harness", "solution", "solution_files", "build", "refs"]) delete ex[k];
      return Object.assign(ex, { workspace: "", file: ex.files[0], progress: p, status: codingStatus(p), skipped: !!p.skipped });
    }
    if (parts[0] === "coding" && parts.length === 3 && post) {
      const ex = await getJson(`data/coding/${parts[1]}.json`);
      const action = parts[2];
      const e = prog.coding[ex.id] = prog.coding[ex.id] || {};
      if (action === "check" && ex.site_unavailable) {   // (the machine of the site cannot grade it: no attempt is counted)
        if (!Object.keys(e).length) delete prog.coding[ex.id];
        return { unavailable: ex.site_unavailable, compiled: false, log: "", cases: [], passed: false, status: codingStatus(prog.coding[ex.id]) };
      }
      if (action === "check") {
        const files = {};
        for (const n of ex.files) files[n] = store.getCode(ex.id, n);
        const empty = ex.files.filter((n) => !files[n].trim());
        const result = empty.length === ex.files.length
          ? { compiled: false, log: `${empty.join(", ")} is empty: write your program there first.`, cases: [], passed: false }
          : await root.CellarVM.check(ex, files);
        e.attempts = (e.attempts || 0) + 1;
        e.last_at = Math.floor(Date.now() / 1000);
        e.last_passed_cases = result.cases.filter((c) => c.ok).length;
        e.last_total_cases = result.cases.length;
        if (result.passed) e.passed = true;
        saveProgress();
        result.status = codingStatus(e);
        return result;
      }
      if (action === "solution") {
        e.solution_viewed = true; saveProgress();
        return { solution: ex.solution, status: codingStatus(e), refs: ex.refs };
      }
      if (action === "skip") {
        if (body.on) e.skipped = true; else delete e.skipped;
        if (!Object.keys(e).length) delete prog.coding[ex.id];
        saveProgress();
        return { skipped: !!body.on };
      }
      if (action === "reset") {
        store.clearCode(ex.id, ex.files);
        if (root.CELLAR_SITE && root.CELLAR_SITE.codeReset) root.CELLAR_SITE.codeReset(ex.id);
        return { ok: true };
      }
    }
    if (parts[0] === "theory" && parts.length === 2 && !post) {
      const s = await getJson(`data/theory/${parts[1]}.json`);
      return { id: s.id, title: s.title, tag: s.tag, chapter: s.chapter, practice: s.practice,
        questions: s.questions.map((q) => Object.assign(publicQuestion(q), { progress: prog.theory[q.id] })) };
    }
    if (parts[0] === "theory" && parts.length === 3 && parts[2] === "answer" && post) {
      const s = await getJson(`data/theory/${parts[1]}.json`);
      const q = s.questions.find((x) => x.id === body.id);
      if (!q) throw new Error("unknown question");
      const correct = grade(q, body.response);
      const e = prog.theory[q.id] = prog.theory[q.id] || {};
      e.attempts = (e.attempts || 0) + 1;
      e.correct_count = (e.correct_count || 0) + (correct ? 1 : 0);
      e.last_correct = correct;
      e.last_at = Math.floor(Date.now() / 1000);
      saveProgress();
      const reply = Object.assign(answerReveal(q), { correct, refs: q.refs, practice: s.practice });
      if (q.type === "fill") reply.blanks_ok = blanksOk(q, body.response);
      else if (q.type === "match" || q.type === "sort") reply.items_ok = pairsOk(q, body.response);
      else if (q.type === "predict" && !correct) reply.hints = predictHints(q, body.response);
      return reply;
    }
    if (route === "reference" && !post) {
      const r = await getJson("data/reference.json");
      return { entries: r.entries, favorites: prog.favorites };
    }
    if (route === "reference/favorite" && post) {
      const on = !!body.on;
      prog.favorites = prog.favorites.filter((x) => x !== body.id);
      if (on) prog.favorites.push(body.id);
      saveProgress();
      return { favorites: prog.favorites };
    }
    if (route === "progress/reset" && post) {
      mem.progress = { coding: {}, theory: {}, favorites: prog.favorites }; saveProgress();
      return { ok: true };
    }
    if (route.startsWith("daily") || route === "readiness") return notHere();
    throw new Error("not found: " + path + (query ? "?" + query : ""));
  }

  root.CELLAR_API = api;
  root.CellarStore = store;
  root.CellarGrade = { grade, blanksOk, pairsOk, outputLines, predictHints, answerReveal, publicQuestion, codingStatus };
  if (typeof module !== "undefined" && module.exports) module.exports = { api, normalizeCustomPath, grade, blanksOk, pairsOk, predictHints, answerReveal, publicQuestion };
})(typeof window !== "undefined" ? window : globalThis);
