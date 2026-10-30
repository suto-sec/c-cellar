/* The site's replacement for the server: window.CELLAR_API(path, body) answers every call app.js makes to /api/ from the data bundle
 * (data/*.json) and the student's progress in localStorage. The theory grading is a port of app/server.py (grade, blanks_ok, pairs_ok...).
 * Code that needs the machine (checking an exercise) goes through window.CellarVM (vm.js). */
(function (root) {
  "use strict";
  const PROGRESS_KEY = "cellar.site.progress.v1";
  const CODE_KEY = "cellar.site.code.v1.";
  const mem = { progress: null, code: {} };   // used when localStorage is not available (a private window...)

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
      const out = { progress: loadProgress(), code: {} };
      try { for (let i = 0; i < ls().length; i++) { const k = ls().key(i); if (k.startsWith(CODE_KEY)) out.code[k] = ls().getItem(k); } } catch (e) { /* nothing */ }
      return out;
    },
    importAll(data) {
      mem.progress = data.progress; saveProgress();
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
      return idx;
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
  if (typeof module !== "undefined" && module.exports) module.exports = { api, grade, blanksOk, pairsOk, predictHints, answerReveal, publicQuestion };
})(typeof window !== "undefined" ? window : globalThis);
