/* c-cellar frontend: hash-routed single page app, no dependencies. */
"use strict";

const $ = (sel, el = document) => el.querySelector(sel);
const app = $("#app");
const state = { tag: localStorage.getItem("cellar.tag") || "t3", cfg: null, cleanup: null };

// ------------------------------------------------------------------ helpers
function h(tag, props, ...kids) {
  const el = document.createElement(tag);
  for (const [k, v] of Object.entries(props || {})) {
    if (v == null || v === false) continue;
    if (k.startsWith("on")) el.addEventListener(k.slice(2), v);
    else if (k === "class") el.className = v;
    else if (k === "html") el.innerHTML = v;
    else if (k in el && k !== "list") el[k] = v;
    else el.setAttribute(k, v === true ? "" : v);
  }
  for (const kid of kids.flat()) {
    if (kid == null || kid === false) continue;
    el.append(kid.nodeType ? kid : document.createTextNode(String(kid)));
  }
  return el;
}

async function api(path, body) {
  const opt = body === undefined ? {} : { method: "POST", headers: { "Content-Type": "application/json" }, body: JSON.stringify(body) };
  const r = await fetch("/api/" + path, opt);
  const j = await r.json();
  if (!r.ok) throw new Error(j.error || r.statusText);
  return j;
}

const esc = (s) => String(s).replace(/&/g, "&amp;").replace(/</g, "&lt;").replace(/>/g, "&gt;");

function toast(msg) {
  const t = h("div", { class: "toast" }, msg);
  document.body.append(t);
  setTimeout(() => t.remove(), 2600);
}

/** Tiny markdown: headings, fenced code, lists, paragraphs, `code`, **bold**, *italic*. Input is repo content, escaped anyway. */
function md(src) {
  const inline = (t) => esc(t).replace(/`([^`]+)`/g, "<code>$1</code>").replace(/\*\*([^*]+)\*\*/g, "<b>$1</b>").replace(/(^|[\s(])\*([^*\s][^*]*)\*/g, "$1<i>$2</i>");
  const out = [];
  const parts = src.split(/^```.*\n([\s\S]*?)^```\s*$/m);
  parts.forEach((part, i) => {
    if (i % 2 === 1) { out.push("<pre><code>" + esc(part.replace(/\n$/, "")) + "</code></pre>"); return; }
    let list = false, para = [];
    const flush = () => { if (para.length) out.push("<p>" + inline(para.join(" ")) + "</p>"); para = []; };
    const closeList = () => { if (list) { out.push("</ul>"); list = false; } };
    for (const line of part.split("\n")) {
      let m;
      if ((m = line.match(/^(#{1,4})\s+(.*)/))) { flush(); closeList(); out.push(`<h${m[1].length}>${inline(m[2])}</h${m[1].length}>`); }
      else if ((m = line.match(/^\s*[-*]\s+(.*)/))) { flush(); if (!list) { out.push("<ul>"); list = true; } out.push("<li>" + inline(m[1]) + "</li>"); }
      else if (!line.trim()) { flush(); closeList(); }
      else { closeList(); para.push(line.trim()); }
    }
    flush(); closeList();
  });
  return out.join("\n");
}

const STATUS = { passed: ["✔", "Passed"], progress: ["●", "In progress"], solution: ["◉", "Solution viewed"], todo: ["○", "Not started"] };
const TRACKS = [["derusting", "C derusting", "You know C but have not used it in a while: small warm-ups."], ["exercises", "C exercises", "Exam-style programs of growing difficulty."]];
const dots = (n) => h("span", { class: "dots", title: "Difficulty " + n + "/3" }, "●".repeat(n), h("i", {}, "●".repeat(3 - n)));
const pct = (x) => Math.round(x * 100);

function setNav(name) {
  document.querySelectorAll("#nav a").forEach((a) => a.classList.toggle("on", a.dataset.nav === name));
}
function mount(...nodes) {
  if (state.cleanup) { state.cleanup(); state.cleanup = null; }
  app.replaceChildren(...nodes);
  app.scrollTop = 0;
}
const page = (...kids) => h("div", { class: "page" }, ...kids);

// ------------------------------------------------------------------ home
async function viewHome() {
  setNav("");
  const [idx, rd] = await Promise.all([api("index"), api("readiness?tag=" + state.tag)]);
  const coding = idx.coding.filter((c) => c.tag === state.tag).sort(byOrder);
  const theory = idx.theory.filter((t) => t.tag === state.tag);
  const passed = coding.filter((c) => c.status === "passed").length;
  const nq = theory.reduce((a, t) => a + t.count, 0), okq = theory.reduce((a, t) => a + t.correct, 0);
  const next = coding.find((c) => c.status !== "passed");
  mount(page(
    h("h1", {}, "c-cellar ", h("span", { class: "tag" }, state.tag)),
    h("p", { class: "lead" }, "Practice C by writing it, and review the theory. Everything is checked locally."),
    h("div", { class: "list" },
      h("a", { class: "row", href: "#/coding" }, h("div", { class: "main" }, h("div", { class: "title" }, "Coding exercises"), h("div", { class: "sub" }, "Derusting warm-ups and exam-style exercises with an automatic checker")), h("span", { class: "pct" }, `${passed}/${coding.length}`)),
      h("a", { class: "row", href: "#/theory" }, h("div", { class: "main" }, h("div", { class: "title" }, "Theory questions"), h("div", { class: "sub" }, "Single choice, multiple choice, fill in the blank and drag to reorder")), h("span", { class: "pct" }, `${okq}/${nq}`)),
      h("a", { class: "row", href: "#/reference" }, h("div", { class: "main" }, h("div", { class: "title" }, "Reference"), h("div", { class: "sub" }, "Searchable C reference: syntax, options, examples, common mistakes")), h("span", { class: "pct" }, String(idx.reference))),
      h("a", { class: "row", href: "#/readiness" }, h("div", { class: "main" }, h("div", { class: "title" }, "Readiness"), h("div", { class: "sub" }, "How prepared you are per topic, and what to review")), h("span", { class: "pct" }, pct(rd.overall) + "%")),
    ),
    next ? h("p", {}, h("a", { class: "btn primary", href: "#/coding/" + next.id }, (passed ? "Continue: " : "Start: ") + next.title)) : null,
  ));
}
const TRACK_ORDER = Object.fromEntries(TRACKS.map(([k], i) => [k, i]));
const byOrder = (a, b) => (TRACK_ORDER[a.track] ?? 9) - (TRACK_ORDER[b.track] ?? 9) || (a.order - b.order) || a.id.localeCompare(b.id);

// ------------------------------------------------------------------ coding list
async function viewCodingList() {
  setNav("coding");
  const idx = await api("index");
  const items = idx.coding.filter((c) => c.tag === state.tag).sort(byOrder);
  const search = h("input", { type: "search", placeholder: "Filter by title or topic…" });
  const holder = h("div", {});
  const draw = () => {
    const q = search.value.toLowerCase();
    holder.replaceChildren(...TRACKS.map(([key, title, blurb]) => {
      const rows = items.filter((c) => c.track === key && (c.title + " " + c.topic + " " + c.summary).toLowerCase().includes(q));
      if (!rows.length) return null;
      const done = items.filter((c) => c.track === key && c.status === "passed").length;
      return h("section", {}, h("h2", {}, title, " ", h("span", { class: "muted" }, `${done}/${items.filter((c) => c.track === key).length}`)),
        h("p", { class: "muted", style: "margin:-6px 0 10px" }, blurb),
        h("div", { class: "list" }, rows.map((c) => h("a", { class: "row", href: "#/coding/" + c.id },
          h("span", { class: "st " + c.status, title: STATUS[c.status][1] }, STATUS[c.status][0]),
          h("div", { class: "main" }, h("div", { class: "title" }, c.title), h("div", { class: "sub" }, c.summary)),
          h("span", { class: "tag" }, c.topic), dots(c.difficulty)))));
    }));
  };
  search.addEventListener("input", draw);
  draw();
  mount(page(h("h1", {}, "Coding exercises"), h("p", { class: "lead" }, "Write your solution in VS Code next to the statement and press Check."), h("div", { class: "tools" }, search), holder));
}

// ------------------------------------------------------------------ exercise screen
function showWs(s) {
  return s.replace(/ +(?=\n|$)/g, (m) => "·".repeat(m.length));
}
function diffPair(got, want) {
  let p = 0;
  while (p < got.length && p < want.length && got[p] === want[p]) p++;
  let q = 0;
  while (q < got.length - p && q < want.length - p && got[got.length - 1 - q] === want[want.length - 1 - q]) q++;
  const render = (s, cls) => {
    const a = s.slice(0, p), b = s.slice(p, s.length - q), c = s.slice(s.length - q);
    const f = (t) => esc(showWs(t)).replace(/\n/g, "↵\n");
    let html = f(a) + (b ? `<mark class="d ${cls}">${f(b)}</mark>` : "") + f(c);
    if (s && !s.endsWith("\n")) html += '<span class="muted">⟨no newline at end⟩</span>';
    return html || '<span class="muted">⟨empty⟩</span>';
  };
  return [render(got, ""), render(want, "e")];
}

function renderCase(c) {
  const kids = [];
  kids.push(h("div", {}, h("span", { class: "muted" }, "run: "), h("code", {}, c.cmd)));
  if (c.stdin != null) kids.push(h("div", {}, h("span", { class: "muted" }, "stdin: "), h("code", {}, JSON.stringify(c.stdin))));
  if (!c.ok) {
    if (c.expected != null) {
      const [g, e] = diffPair(c.got || "", c.expected);
      kids.push(h("div", { class: "cols" }, h("div", {}, h("h4", {}, "Yours"), h("pre", { html: g })), h("div", {}, h("h4", {}, "Expected"), h("pre", { html: e }))));
    } else {
      kids.push(h("div", {}, h("h4", {}, "Yours"), h("pre", {}, c.got || "⟨empty⟩")), h("div", { class: "muted" }, "(expected output hidden)"));
    }
    (c.hints || []).forEach((t) => kids.push(h("div", { class: "hint" }, "💡 " + t)));
    if (c.stderr && c.stderr.trim()) kids.push(h("div", {}, h("h4", { class: "muted", style: "margin:8px 0 2px;font-size:12px" }, "STDERR"), h("pre", {}, c.stderr)));
  }
  return h("details", { class: "case " + (c.ok ? "ok" : "fail"), open: !c.ok }, h("summary", {}, h("span", { class: "mark" }, c.ok ? "✔" : "✘"), c.name), h("div", { class: "body" }, kids));
}

async function viewExercise(id) {
  setNav("coding");
  const [d, idx] = await Promise.all([api("coding/" + id), api("index")]);
  const list = idx.coding.filter((c) => c.tag === d.tag).sort(byOrder);
  const pos = list.findIndex((c) => c.id === id);
  const prev = list[pos - 1], next = list[pos + 1];
  const chip = h("span", { class: "status-chip" });
  const setStatus = (s) => { chip.replaceChildren(h("span", { class: "st " + s }, STATUS[s][0]), STATUS[s][1]); };
  setStatus(d.status);
  const results = h("div", {});
  const extras = h("div", {});
  const checkBtn = h("button", { class: "btn primary", title: "Ctrl+Enter" }, "Check ", h("kbd", {}, "Ctrl+Enter"));
  let hintsShown = 0;
  const hintBtn = h("button", { class: "btn" }, "Hint");
  const hintBox = h("div", {});
  hintBtn.addEventListener("click", () => {
    if (!d.hints.length) return toast("No hints for this exercise");
    if (hintsShown < d.hints.length) hintBox.append(h("div", { class: "hint" }, `Hint ${hintsShown + 1}/${d.hints.length}: `, d.hints[hintsShown++]));
    if (hintsShown >= d.hints.length) hintBtn.disabled = true;
  });
  const infoBtn = h("button", { class: "btn" }, "Info");
  infoBtn.addEventListener("click", () => {
    const old = $(".infobox", extras);
    if (old) old.remove(); else extras.prepend(h("div", { class: "panel infobox", html: md(d.info || "No extra info.") }));
  });
  const solBtn = h("button", { class: "btn" }, "Show solution");
  solBtn.addEventListener("click", async () => {
    const st = (await api("coding/" + id)).status;
    if (st !== "passed" && !confirm("Show the solution? The exercise will be marked “solution viewed” until you pass it.")) return;
    const r = await api("coding/" + id + "/solution", {});
    setStatus(r.status);
    const old = $(".solbox", extras);
    if (old) old.remove();
    extras.append(h("div", { class: "panel solbox" }, h("b", {}, "Reference solution"), h("pre", {}, h("code", {}, r.solution))));
  });
  const resetBtn = h("button", { class: "btn" }, "Reset file");
  resetBtn.addEventListener("click", async () => {
    if (!confirm("Replace your answer.c with the original starter? Your code will be lost.")) return;
    await api("coding/" + id + "/reset", {});
    toast("answer.c restored");
  });
  let busy = false;
  const check = async () => {
    if (busy) return;
    busy = true; checkBtn.disabled = true;
    results.replaceChildren(h("div", { class: "muted" }, "Compiling and running…"));
    try {
      const r = await api("coding/" + id + "/check", {});
      setStatus(r.status);
      const nodes = [];
      if (!r.compiled) {
        nodes.push(h("div", { class: "panel bad" }, h("b", {}, "✘ It does not compile"), h("pre", { class: "compile-log" }, r.log)));
      } else {
        const ok = r.cases.filter((c) => c.ok).length;
        nodes.push(h("div", { class: "panel " + (r.passed ? "ok" : "bad") }, h("b", {}, r.passed ? `✔ All ${r.cases.length} checks passed` : `✘ ${ok}/${r.cases.length} checks passed`),
          r.passed && next ? h("span", {}, " — ", h("a", { href: "#/coding/" + next.id }, "next: " + next.title)) : null));
        if (r.log.trim()) nodes.push(h("details", { class: "case" }, h("summary", {}, h("span", { class: "mark", style: "color:var(--warn)" }, "!"), "Compiler warnings (fix them)"), h("div", { class: "body" }, h("pre", { class: "compile-log" }, r.log))));
        r.cases.forEach((c) => nodes.push(renderCase(c)));
      }
      results.replaceChildren(...nodes);
    } catch (e) {
      results.replaceChildren(h("div", { class: "panel bad" }, "Error: " + e.message));
    }
    busy = false; checkBtn.disabled = false;
  };
  checkBtn.addEventListener("click", check);
  const onKey = (e) => { if ((e.ctrlKey || e.metaKey) && e.key === "Enter") { e.preventDefault(); check(); } };
  document.addEventListener("keydown", onKey);

  const payload = encodeURIComponent(JSON.stringify([["openFile", "vscode-remote://" + d.file]]));
  const url = `${location.protocol}//${location.hostname}:${state.cfg.vscode_port}/?folder=${encodeURIComponent(d.workspace)}&payload=${payload}`;
  const frame = h("iframe", { src: url, title: "VS Code", allow: "clipboard-read; clipboard-write" });
  const left = h("div", { class: "left statement" },
    h("div", { class: "crumbs" }, h("a", { href: "#/coding" }, "Coding"), " › ", d.track === "derusting" ? "C derusting" : "C exercises", " · ", h("span", { class: "tag" }, d.tag), " · ", h("span", { class: "tag" }, d.topic), " ", dots(d.difficulty)),
    h("div", { html: md(d.statement) }),
    h("div", { class: "muted", style: "font-size:13px" }, "Edit ", h("code", {}, "answer.c"), " in the editor on the right (folder ", h("code", {}, d.workspace.split("/").slice(-2).join("/")), ")."),
    h("div", { class: "actions" }, checkBtn, hintBtn, infoBtn, solBtn, resetBtn, h("span", { class: "spacer" }), chip),
    hintBox, results, extras,
    h("div", { class: "qnav" }, prev ? h("a", { class: "btn small", href: "#/coding/" + prev.id }, "← " + prev.title) : null, h("span", { class: "spacer" }), next ? h("a", { class: "btn small", href: "#/coding/" + next.id }, next.title + " →") : null));
  const divider = h("div", { class: "divider", title: "Drag to resize" });
  const right = h("div", { class: "right" }, frame);
  const w = parseFloat(localStorage.getItem("cellar.split")) || 46;
  left.style.width = w + "%";
  divider.addEventListener("pointerdown", (e) => {
    divider.setPointerCapture(e.pointerId);
    divider.classList.add("drag");
    frame.style.pointerEvents = "none";
    const move = (ev) => {
      const box = split.getBoundingClientRect();
      const p = Math.min(80, Math.max(20, ((ev.clientX - box.left) / box.width) * 100));
      left.style.width = p + "%";
      localStorage.setItem("cellar.split", String(p));
    };
    const up = () => { divider.classList.remove("drag"); frame.style.pointerEvents = ""; divider.removeEventListener("pointermove", move); divider.removeEventListener("pointerup", up); };
    divider.addEventListener("pointermove", move);
    divider.addEventListener("pointerup", up);
  });
  const split = h("div", { class: "split" }, left, divider, right);
  mount(split);
  state.cleanup = () => document.removeEventListener("keydown", onKey);
}

// ------------------------------------------------------------------ theory
async function viewTheoryList() {
  setNav("theory");
  const idx = await api("index");
  const sets = idx.theory.filter((t) => t.tag === state.tag);
  mount(page(h("h1", {}, "Theory"), h("p", { class: "lead" }, "Short questions with an explanation after every answer."),
    h("div", { class: "list" }, sets.map((s) => {
      const p = s.count ? s.correct / s.count : 0;
      return h("a", { class: "row", href: "#/theory/" + s.id },
        h("div", { class: "main" }, h("div", { class: "title" }, s.title), h("div", { class: "sub" }, `${s.count} questions · ${s.types.join(", ")} · ${s.answered} answered`)),
        h("div", { class: "bar", style: "width:120px" }, h("span", { style: `width:${pct(p)}%` })), h("span", { class: "pct" }, pct(p) + "%"));
    }))));
}

const TYPE_LABEL = { single: "Single choice", multiple: "Multiple choice (select all that apply)", fill: "Fill in the blank", order: "Put in order" };

/** Builds the answer widget for a question. Returns { el, get(), lock(result) }. */
function buildQuestion(q, onChange) {
  if (q.type === "single" || q.type === "multiple") {
    const multi = q.type === "multiple";
    const name = "q" + Math.random().toString(36).slice(2);
    const inputs = q.options.map((o, i) => h("input", { type: multi ? "checkbox" : "radio", name, value: String(i), onchange: onChange }));
    const labels = q.options.map((o, i) => h("label", { class: "opt" }, inputs[i], h("span", {}, h("span", { html: esc(o).replace(/`([^`]+)`/g, "<code>$1</code>") }))));
    const box = h("div", { class: "opts" }, labels);
    inputs.forEach((inp, i) => inp.addEventListener("change", () => labels.forEach((l, j) => l.classList.toggle("sel", inputs[j].checked))));
    return {
      el: box,
      get: () => (multi ? inputs.map((x, i) => (x.checked ? i : -1)).filter((i) => i >= 0) : inputs.findIndex((x) => x.checked)),
      ready: () => inputs.some((x) => x.checked),
      lock(r) {
        box.classList.add("done");
        const right = new Set(multi ? r.answer : [r.answer]);
        inputs.forEach((x, i) => {
          x.disabled = true;
          labels[i].classList.remove("sel");
          if (right.has(i)) labels[i].classList.add("right"); else if (x.checked) labels[i].classList.add("wrong");
          if (r.option_notes && r.option_notes[i]) labels[i].firstChild.nextSibling.append(h("span", { class: "note", html: esc(r.option_notes[i]).replace(/`([^`]+)`/g, "<code>$1</code>") }));
        });
      },
      focus: () => inputs[0].focus(),
    };
  }
  if (q.type === "fill") {
    const inputs = [];
    const frag = [];
    q.__promptParts.forEach((part) => {
      if (typeof part === "string") frag.push(h("span", { html: esc(part).replace(/`([^`]+)`/g, "<code>$1</code>") }));
      else { const inp = h("input", { type: "text", autocomplete: "off", spellcheck: false, size: 12, oninput: onChange }); inputs[part] = inp; frag.push(inp); }
    });
    return {
      el: h("div", { class: "fill" }, frag),
      get: () => inputs.map((i) => i.value),
      ready: () => inputs.every((i) => i.value.trim()),
      lock(r) {
        inputs.forEach((inp, i) => { inp.disabled = true; inp.classList.add(r.blanks_ok[i] ? "right" : "wrong"); });
      },
      focus: () => inputs[0].focus(),
      promptInline: true,
    };
  }
  // order
  const ul = h("ul", { class: "order" });
  let items = q.items.slice();
  let dragged = null, locked = false;
  const draw = () => {
    ul.replaceChildren(...items.map((it, i) => {
      const li = h("li", { draggable: true, "data-id": it.id },
        h("span", { class: "grip" }, "⋮⋮"), h("span", { class: "num" }, String(i + 1)), h("span", { html: esc(it.text).replace(/`([^`]+)`/g, "<code>$1</code>") }),
        h("span", { class: "mv" },
          h("button", { title: "Move up", "aria-label": "Move up", onclick: () => move(i, i - 1) }, "▲"),
          h("button", { title: "Move down", "aria-label": "Move down", onclick: () => move(i, i + 1) }, "▼")));
      li.addEventListener("dragstart", (e) => { if (locked) return e.preventDefault(); dragged = i; li.classList.add("dragging"); e.dataTransfer.effectAllowed = "move"; e.dataTransfer.setData("text/plain", String(i)); });
      li.addEventListener("dragend", () => { li.classList.remove("dragging"); ul.querySelectorAll(".over").forEach((x) => x.classList.remove("over")); });
      li.addEventListener("dragover", (e) => { if (dragged == null) return; e.preventDefault(); ul.querySelectorAll(".over").forEach((x) => x.classList.remove("over")); li.classList.add("over"); });
      li.addEventListener("drop", (e) => { e.preventDefault(); if (dragged != null) move(dragged, i); dragged = null; });
      return li;
    }));
  };
  const move = (from, to) => {
    if (locked || to < 0 || to >= items.length || from === to) return;
    const [x] = items.splice(from, 1);
    items.splice(to, 0, x);
    draw(); onChange();
  };
  draw();
  return {
    el: ul,
    get: () => items.map((i) => i.id),
    ready: () => true,
    lock(r) {
      locked = true; ul.classList.add("done");
      ul.querySelectorAll("li").forEach((li, i) => { li.draggable = false; li.querySelector(".mv").remove(); li.classList.add("opt"); li.classList.add(items[i].id === i ? "right" : "wrong"); });
      if (!r.correct) ul.after(h("div", { class: "muted", style: "margin:6px 0" }, "Correct order: ", h("ol", {}, r.answer.map((t) => h("li", {}, t)))));
    },
    focus: () => {},
  };
}

async function viewTheorySet(id) {
  setNav("theory");
  const set = await api("theory/" + id);
  const answered = set.questions.filter((q) => q.progress).length;
  const notCorrect = set.questions.filter((q) => !q.progress || !q.progress.last_correct);
  const missed = set.questions.filter((q) => q.progress && !q.progress.last_correct);
  const start = (qs, shuffle) => {
    const queue = shuffle ? qs.slice().sort(() => Math.random() - 0.5) : qs.slice();
    run(queue);
  };
  const intro = () => {
    const shuffle = h("input", { type: "checkbox", checked: false });
    mount(page(
      h("div", { class: "crumbs" }, h("a", { href: "#/theory" }, "Theory"), " › ", set.title),
      h("h1", {}, set.title), h("p", { class: "lead" }, `${set.questions.length} questions · ${answered} answered before`),
      h("div", { class: "panel" }, h("label", {}, shuffle, " Shuffle the order")),
      h("p", {},
        h("button", { class: "btn primary", onclick: () => start(set.questions, shuffle.checked) }, "Start all"), " ",
        h("button", { class: "btn", disabled: !notCorrect.length || notCorrect.length === set.questions.length, onclick: () => start(notCorrect, shuffle.checked) }, `Only not yet correct (${notCorrect.length})`), " ",
        h("button", { class: "btn", disabled: !missed.length, onclick: () => start(missed, shuffle.checked) }, `Only last missed (${missed.length})`))));
  };
  const run = (queue) => {
    let i = 0, right = 0;
    const missedNow = [];
    const showQuestion = () => {
      const q = queue[i];
      q.__promptParts = q.type === "fill" ? q.prompt.split(/\{\{(\d+)\}\}/).map((p, k) => (k % 2 ? Number(p) : p)) : null;
      const feedback = h("div", {});
      const checkBtn = h("button", { class: "btn primary" }, "Check");
      const nextBtn = h("button", { class: "btn primary", style: "display:none" }, i + 1 < queue.length ? "Next →" : "Finish");
      let widget, done = false;
      const refresh = () => { checkBtn.disabled = done || !widget.ready(); };
      widget = buildQuestion(q, refresh);
      const submit = async () => {
        if (done || !widget.ready()) return;
        done = true; checkBtn.disabled = true;
        const r = await api("theory/" + id + "/answer", { id: q.id, response: widget.get() });
        widget.lock(r);
        if (r.correct) right++; else missedNow.push(q);
        feedback.replaceChildren(h("div", { class: "explain " + (r.correct ? "ok" : "bad") },
          h("div", { class: "verdict " + (r.correct ? "ok" : "bad") }, r.correct ? "✔ Correct" : "✘ Not quite"),
          h("div", { html: md(r.explain) })));
        checkBtn.style.display = "none"; nextBtn.style.display = ""; nextBtn.focus();
      };
      checkBtn.addEventListener("click", submit);
      nextBtn.addEventListener("click", () => { i++; i < queue.length ? showQuestion() : summary(); });
      const onKey = (e) => {
        if (e.key !== "Enter" || e.target.tagName === "BUTTON" || e.shiftKey) return;
        e.preventDefault();
        done ? nextBtn.click() : submit();
      };
      document.addEventListener("keydown", onKey);
      const prompt = widget.promptInline ? null : h("div", { class: "qprompt", html: esc(q.prompt).replace(/`([^`]+)`/g, "<code>$1</code>") });
      mount(page(
        h("div", { class: "crumbs" }, h("a", { href: "#/theory" }, "Theory"), " › ", h("a", { href: "#/theory/" + id }, set.title)),
        h("div", { class: "bar qprog" }, h("span", { style: `width:${(i / queue.length) * 100}%` })),
        h("div", { class: "qcard" },
          h("div", { class: "qmeta" }, h("span", {}, `Question ${i + 1} of ${queue.length}`), h("span", { class: "tag" }, TYPE_LABEL[q.type]), h("span", { class: "tag" }, q.topic), dots(q.difficulty || 1)),
          prompt, q.code ? h("pre", {}, h("code", {}, q.code)) : null, widget.el, feedback,
          h("div", { class: "qnav" }, checkBtn, nextBtn))));
      state.cleanup = () => document.removeEventListener("keydown", onKey);
      refresh();
      setTimeout(() => widget.focus && widget.focus(), 0);
    };
    const summary = () => {
      mount(page(h("div", { class: "center" },
        h("div", { class: "big" }, `${right}/${queue.length}`),
        h("p", { class: "lead" }, right === queue.length ? "Perfect round." : "Review what you missed, then try again."),
        missedNow.length ? h("div", { class: "list", style: "text-align:left;margin:16px 0" }, missedNow.map((q) => h("div", { class: "row" }, h("div", { class: "main" }, h("div", { class: "title" }, q.prompt.replace(/\{\{\d+\}\}/g, "____")), h("div", { class: "sub" }, TYPE_LABEL[q.type]))))) : null,
        h("p", {}, missedNow.length ? h("button", { class: "btn primary", onclick: () => run(missedNow.slice()) }, "Retry the missed ones") : null, " ",
          h("a", { class: "btn", href: "#/theory" }, "Back to theory"), " ", h("button", { class: "btn", onclick: () => viewTheorySet(id) }, "Restart")))));
    };
    showQuestion();
  };
  intro();
}

// ------------------------------------------------------------------ reference
async function viewReference(focusId) {
  setNav("reference");
  const all = (await api("reference")).entries.filter((e) => e.tag === state.tag);
  const cats = [...new Set(all.map((e) => e.category))];
  let cat = null;
  const search = h("input", { type: "search", placeholder: "Search: open, fgets, %zu, dup2, LD_LIBRARY_PATH…", autofocus: true });
  const list = h("div", {});
  const catBox = h("div", { class: "refcats" });
  const text = (e) => [e.title, e.summary, e.syntax, e.header, e.description, e.example, e.category, (e.details || []).map((d) => d.name + " " + d.text).join(" "), (e.mistakes || []).join(" ")].join(" ").toLowerCase();
  const index = new Map(all.map((e) => [e.id, text(e)]));
  const draw = () => {
    const q = search.value.trim().toLowerCase();
    const terms = q.split(/\s+/).filter(Boolean);
    const shown = all.filter((e) => (!cat || e.category === cat) && terms.every((t) => index.get(e.id).includes(t)));
    shown.sort((a, b) => (b.title.toLowerCase().includes(q) ? 1 : 0) - (a.title.toLowerCase().includes(q) ? 1 : 0));
    catBox.replaceChildren(h("a", { href: "javascript:void 0", class: !cat ? "on" : "", onclick: () => { cat = null; draw(); } }, "All"), ...cats.map((c) => h("a", { href: "javascript:void 0", class: cat === c ? "on" : "", onclick: () => { cat = c; draw(); } }, c)));
    list.replaceChildren(...(shown.length ? shown.map(entryCard) : [h("p", { class: "muted" }, "No entries match.")]));
  };
  const entryCard = (e) => h("article", { class: "entry" + (e.id === focusId ? " hl" : ""), id: e.id },
    e.header ? h("span", { class: "hdr" }, e.header) : null,
    h("h3", {}, e.title), h("div", { class: "cat" }, e.category),
    h("p", {}, e.summary),
    e.syntax ? h("pre", {}, h("code", {}, e.syntax)) : null,
    e.description ? h("div", { html: e.description.split("\n").map((p) => "<p>" + esc(p).replace(/`([^`]+)`/g, "<code>$1</code>") + "</p>").join("") }) : null,
    e.details ? h("dl", {}, e.details.flatMap((d) => [h("dt", {}, d.name), h("dd", { html: esc(d.text).replace(/`([^`]+)`/g, "<code>$1</code>") })])) : null,
    e.example ? [h("h5", {}, "Example"), h("pre", {}, h("code", {}, e.example))] : null,
    e.mistakes ? [h("h5", {}, "Common mistakes"), h("ul", {}, e.mistakes.map((m) => h("li", { html: esc(m).replace(/`([^`]+)`/g, "<code>$1</code>") })))] : null,
    e.see ? h("div", { class: "muted", style: "margin-top:8px" }, "See also: ", e.see.map((s) => { const t = all.find((x) => x.id === s); return t ? [h("a", { href: "#/reference/" + s }, t.title), " "] : null; })) : null);
  search.addEventListener("input", draw);
  draw();
  mount(page(h("h1", {}, "Reference"), h("p", { class: "lead" }, `${all.length} entries for ${state.tag}. Every word you type must match.`), h("div", { class: "tools" }, search),
    h("div", { class: "refgrid" }, catBox, list)));
  const target = focusId && document.getElementById(focusId);
  if (target) target.scrollIntoView(); else search.focus();
}

// ------------------------------------------------------------------ readiness
async function viewReadiness() {
  setNav("readiness");
  const r = await api("readiness?tag=" + state.tag);
  const cls = (s) => (s >= 0.75 ? "" : s >= 0.4 ? "warn" : "bad");
  mount(page(h("h1", {}, "Readiness ", h("span", { class: "tag" }, state.tag)),
    h("p", { class: "lead" }, "Coding exercises passed (half credit if you looked at the solution) and theory questions whose last answer was correct, per topic. Weakest first."),
    h("div", { class: "panel" }, h("div", { style: "display:flex;gap:14px;align-items:center" }, h("div", { class: "big" }, pct(r.overall) + "%"), h("div", { class: "bar " + cls(r.overall), style: "flex:1;height:12px" }, h("span", { style: `width:${pct(r.overall)}%` })))),
    h("div", { class: "list" }, r.topics.map((t) => h("div", { class: "row" },
      h("div", { class: "main" }, h("div", { class: "title" }, t.topic), h("div", { class: "sub" }, `coding ${t.coding_score}/${t.coding_total} · theory ${t.theory_score}/${t.theory_total}`)),
      h("div", { class: "bar " + cls(t.score), style: "width:160px" }, h("span", { style: `width:${pct(t.score)}%` })), h("span", { class: "pct" }, pct(t.score) + "%")))),
    h("h2", {}, "Questions to review"),
    r.missed.length ? h("div", { class: "list" }, r.missed.map((m) => h("a", { class: "row", href: "#/theory/" + m.set }, h("div", { class: "main" }, h("div", { class: "title" }, m.prompt.replace(/\{\{\d+\}\}/g, "____")), h("div", { class: "sub" }, m.topic))))) : h("p", { class: "muted" }, "Nothing missed so far."),
    h("p", { style: "margin-top:30px" }, h("button", { class: "btn small", onclick: async () => { if (confirm("Erase all progress (exercise status and theory answers)? Your answer.c files are kept.")) { await api("progress/reset", {}); viewReadiness(); } } }, "Reset progress"))));
}

// ------------------------------------------------------------------ router
async function route() {
  const [, a, b] = location.hash.split("/");
  try {
    if (!a) await viewHome();
    else if (a === "coding") await (b ? viewExercise(b) : viewCodingList());
    else if (a === "theory") await (b ? viewTheorySet(b) : viewTheoryList());
    else if (a === "reference") await viewReference(b);
    else if (a === "readiness") await viewReadiness();
    else await viewHome();
  } catch (e) {
    mount(page(h("h1", {}, "Something went wrong"), h("pre", {}, String(e.message || e))));
  }
}

async function init() {
  const theme = localStorage.getItem("cellar.theme");
  if (theme) document.documentElement.dataset.theme = theme;
  $("#theme").addEventListener("click", () => {
    const dark = getComputedStyle(document.documentElement).getPropertyValue("--bg").trim() === "#17140f";
    const next = dark ? "light" : "dark";
    document.documentElement.dataset.theme = next;
    localStorage.setItem("cellar.theme", next);
  });
  state.cfg = await api("config");
  if (!state.cfg.tags.includes(state.tag)) state.tag = state.cfg.tags[0];
  const sel = $("#tag");
  sel.replaceChildren(...state.cfg.tags.map((t) => h("option", { value: t, selected: t === state.tag }, t)));
  sel.addEventListener("change", () => { state.tag = sel.value; localStorage.setItem("cellar.tag", state.tag); route(); });
  window.addEventListener("hashchange", route);
  route();
}
init();
