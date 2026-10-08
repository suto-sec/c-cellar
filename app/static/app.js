/* c-cellar frontend: hash-routed single page app, no dependencies. */
"use strict";

const $ = (sel, el = document) => el.querySelector(sel);

// replaceChildren turns null/false into the text "null"/"false" and arrays into "[object ...]": skip empty values and flatten arrays
const nativeReplaceChildren = Element.prototype.replaceChildren;
Element.prototype.replaceChildren = function (...kids) {
  return nativeReplaceChildren.apply(this, kids.flat(Infinity).filter((k) => k != null && k !== false));
};
const app = $("#app");
const state = { cfg: null, cleanup: null, query: new URLSearchParams() };
const T = (tag) => String(tag).toUpperCase();   // "t3" is shown as "T3" (the course topic, "tema")
const FILTER_KEYS = { coding: "cellar.filters.coding", theory: "cellar.filters.theory", reference: "cellar.filters.reference" };
function readFilters(key) {
  try { const f = JSON.parse(localStorage.getItem(key)); return { tags: f.tags || [], levels: f.levels || [] }; } catch (e) { return { tags: [], levels: [] }; }
}

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
  for (const kid of kids.flat(Infinity)) {
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
  const inline = (t) => {   // code spans are lifted out first, so `*/` or `**` inside code never turns into bold/italic
    const codes = [];
    const x = esc(t).replace(/`([^`]+)`/g, (m, c) => { codes.push(c); return `\u0002${codes.length - 1}\u0003`; })
      .replace(/\*\*([^*]+)\*\*/g, "<b>$1</b>").replace(/(^|[\s(])\*([^*\s][^*]*)\*/g, "$1<i>$2</i>");
    return x.replace(/\u0002(\d+)\u0003/g, (m, n) => `<code>${codes[n]}</code>`);
  };
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

const ic = (t) => esc(t).replace(/`([^`]+)`/g, "<code>$1</code>");   // text with `code` spans
const STATUS = { passed: ["✔", "Passed"], progress: ["●", "In progress"], solution: ["◉", "Solution viewed"], todo: ["○", "Not started"] };
const TRACKS = [["derusting", "C derusting", "You know C but have not used it in a while: small drills, chapter by chapter, from 1 to 5 stars."], ["exercises", "C exercises", "Exam-style programs that combine several chapters."]];
const stars = (n) => h("span", { class: "stars", title: "Difficulty " + n + "/5" }, "★".repeat(n), h("i", {}, "★".repeat(5 - n)));
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


/** The filter bar every page uses: Topic chips (the course topic T3, T4...) and, when the page has difficulty levels, Difficulty chips.
 *  Several chips of a group can be on at once; none on means "all". The choice is remembered per page. */
function makeFilters(key, { levels = [], levelLabel = (n) => String(n), levelTitle = (n) => "Level " + n, onChange }) {
  const f = readFilters(key);
  const el = h("div", { class: "filters" });
  const save = () => localStorage.setItem(key, JSON.stringify(f));
  const toggle = (list, v) => { const i = list.indexOf(v); if (i >= 0) list.splice(i, 1); else list.push(v); save(); draw(); onChange(); };
  const chip = (label, on, onclick, title) => h("button", { class: "chip" + (on ? " on" : ""), "aria-pressed": String(on), title, onclick }, label);
  const draw = () => {
    el.replaceChildren(
      h("div", { class: "fgroup" }, h("span", { class: "flabel" }, "Topic"), state.cfg.tags.map((t) => chip(T(t), f.tags.includes(t), () => toggle(f.tags, t), "Show only topic " + T(t)))),
      levels.length ? h("div", { class: "fgroup" }, h("span", { class: "flabel" }, "Difficulty"), levels.map((n) => chip(levelLabel(n), f.levels.includes(n), () => toggle(f.levels, n), levelTitle(n)))) : null,
      f.tags.length || f.levels.length ? h("button", { class: "btn small", onclick: () => { f.tags = []; f.levels = []; save(); draw(); onChange(); } }, "Clear filters") : null);
  };
  draw();
  return { f, el };
}
const uniqueSorted = (list) => [...new Set(list)].sort((a, b) => a - b);

// ------------------------------------------------------------------ home
const byOrder = (a, b) => (a.order - b.order) || a.id.localeCompare(b.id);
/** Coding data for the given tags (none = all): chapters with their exercises, the suggested path and the next exercise to do. */
function codingModel(idx, tags = []) {
  const want = (t) => !tags.length || tags.includes(t);
  const items = idx.coding.filter((c) => want(c.tag));
  const chapters = idx.chapters.filter((c) => want(c.tag)).map((c) => ({ ...c, key: c.tag + "/" + c.id, items: items.filter((i) => i.tag === c.tag && i.topic === c.id).sort(byOrder) })).filter((c) => c.items.length);
  const path = Object.keys(idx.paths).filter(want).flatMap((t) => idx.paths[t]).map((id) => items.find((i) => i.id === id)).filter(Boolean);
  const order = path.length ? path : chapters.flatMap((c) => c.items);
  const next = order.find((i) => i.status !== "passed");
  return { items, chapters, order, next, passed: items.filter((i) => i.status === "passed").length };
}

async function viewHome() {
  setNav("");
  const idx = await api("index");
  const m = codingModel(idx);
  const nq = idx.theory.reduce((a, t) => a + t.count, 0), okq = idx.theory.reduce((a, t) => a + t.correct, 0);
  mount(page(
    h("h1", {}, "c-cellar"),
    h("p", { class: "lead" }, "Practice C by writing it, and review the theory. Everything is checked locally."),
    h("div", { class: "list" },
      h("a", { class: "row", href: "#/coding" }, h("div", { class: "main" }, h("div", { class: "title" }, "Coding exercises"), h("div", { class: "sub" }, `${m.chapters.length} chapters from 1 to 5 stars, with an automatic checker`)), h("span", { class: "pct" }, `${m.passed}/${m.items.length}`)),
      h("a", { class: "row", href: "#/theory" }, h("div", { class: "main" }, h("div", { class: "title" }, "Theory questions"), h("div", { class: "sub" }, "Choose, type, predict the output, match, sort or put in order: with an explanation after every answer")), h("span", { class: "pct" }, `${okq}/${nq}`)),
      h("a", { class: "row", href: "#/reference" }, h("div", { class: "main" }, h("div", { class: "title" }, "Reference"), h("div", { class: "sub" }, "One entry per function, command and keyword: syntax, options, examples, common mistakes")), h("span", { class: "pct" }, String(idx.reference))),
    ),
    m.next ? h("p", {}, h("a", { class: "btn primary", href: "#/coding/" + m.next.id }, (m.passed ? "Continue: " : "Start: ") + m.next.title)) : null,
  ));
}

// ------------------------------------------------------------------ coding list
async function viewCodingList() {
  setNav("coding");
  const idx = await api("index");
  const search = h("input", { type: "search", placeholder: "Filter by title or chapter…" });
  const holder = h("div", {});
  const pathHolder = h("div", {});
  const openState = new Map();
  const wanted = state.query.get("chapter");   // a link from a theory set or question: show that chapter, whatever the filters were
  if (wanted) { localStorage.removeItem(FILTER_KEYS.coding); openState.set(wanted, true); }
  const bar = makeFilters(FILTER_KEYS.coding, { levels: uniqueSorted(idx.coding.map((c) => c.stars)), levelLabel: (n) => "★".repeat(n), levelTitle: (n) => n + (n === 1 ? " star" : " stars"), onChange: () => render() });
  const filters = bar.f;

  const render = () => {
    const m = codingModel(idx, filters.tags);
    const q = search.value.toLowerCase();
    const narrowed = q || filters.levels.length;
    const keep = (c, i) => (!filters.levels.length || filters.levels.includes(i.stars)) && (!q || (i.title + " " + c.title + " " + i.summary).toLowerCase().includes(q));
    let shown = 0;
    holder.replaceChildren(...TRACKS.map(([key, title, blurb]) => {
      const chs = m.chapters.filter((c) => c.track === key);
      if (!chs.length) return null;
      const sections = chs.map((c) => {
        const rows = c.items.filter((i) => keep(c, i));
        if (!rows.length) return null;
        shown += rows.length;
        const done = c.items.filter((i) => i.status === "passed").length;
        const isNext = m.next && c.items.includes(m.next);
        const det = h("details", { class: "chapter", "data-key": c.key, open: narrowed ? true : openState.has(c.key) ? openState.get(c.key) : isNext || done === 0 && c === chs[0] },
          h("summary", {}, h("span", { class: "ctitle" }, c.title), h("span", { class: "muted csub" }, c.blurb), narrowed ? h("span", { class: "muted", style: "font-size:13px" }, `${rows.length} shown`) : null, h("span", { class: "cprog" }, `${done}/${c.items.length}`),
            h("span", { class: "bar", style: "width:70px" }, h("span", { style: `width:${(done / c.items.length) * 100}%` }))),
          h("div", { class: "list" }, rows.map((i) => h("a", { class: "row", href: "#/coding/" + i.id },
            h("span", { class: "st " + i.status, title: STATUS[i.status][1] }, STATUS[i.status][0]),
            h("div", { class: "main" }, h("div", { class: "title" }, i.title), h("div", { class: "sub" }, i.summary)), h("span", { class: "tag", title: "Course topic " + T(i.tag) }, T(i.tag)), stars(i.stars)))));
        det.addEventListener("toggle", () => openState.set(c.key, det.open));
        return det;
      });
      if (!sections.some(Boolean)) return null;   // nothing of this track matches the filters
      const inTrack = chs.flatMap((c) => c.items);
      return h("section", {}, h("h2", {}, title, " ", h("span", { class: "muted" }, `${inTrack.filter((i) => i.status === "passed").length}/${inTrack.length}`)),
        h("p", { class: "muted", style: "margin:-6px 0 10px" }, blurb), sections);
    }));
    if (!shown) holder.append(h("p", { class: "muted" }, "No exercise matches these filters."));
    pathHolder.replaceChildren(m.order.length ? h("div", { class: "panel", style: "display:flex;gap:14px;align-items:center;flex-wrap:wrap" },
      h("div", { style: "flex:1;min-width:220px" }, h("b", {}, "Suggested path"), h("div", { class: "muted", style: "font-size:13px" }, `${m.passed} of ${m.items.length} passed. The path mixes the chapters so the difficulty climbs gradually.`),
        h("div", { class: "bar", style: "margin-top:6px" }, h("span", { style: `width:${(m.passed / m.items.length) * 100}%` }))),
      m.next ? h("a", { class: "btn primary", href: "#/coding/" + m.next.id }, (m.passed ? "Continue: " : "Start: ") + m.next.title) : h("b", {}, "Everything passed ✔")) : null);
  };
  search.addEventListener("input", render);
  render();
  setTimeout(() => { const t = wanted && document.querySelector(`details.chapter[data-key="${wanted}"]`); if (t) t.scrollIntoView({ block: "start" }); }, 0);
  mount(page(h("h1", {}, "Coding exercises"), h("p", { class: "lead" }, "Write your solution in VS Code (the terminal is next to it) beside the statement and press Check. Use Hint if you are stuck, and Answer as a last resort."), pathHolder, bar.el, h("div", { class: "tools" }, search), holder));
}

// ------------------------------------------------------------------ dock: statement, terminal and VS Code on the exercise screen
// The three panels are the leaves of a tree of splits ({dir: "row"|"col", ratio, a, b}). A prebuilt layout is a ready-made tree; dragging a
// panel by its header or a splitter edits the tree, which becomes the "Custom" layout. Only the panels' left/top/width/height change, so the
// iframes are never moved in the DOM (that would reload them: the editor and the shell would restart).
const PANELS = { instr: "Statement", term: "Terminal", code: "VS Code" };
const leaf = (panel) => ({ panel });
const splitOf = (dir, ratio, a, b) => ({ dir, ratio, a, b });
const LAYOUTS = [
  { id: "default", title: "Default", hint: "Statement on the left; VS Code over the terminal", tree: () => splitOf("row", 42, leaf("instr"), splitOf("col", 62, leaf("code"), leaf("term"))) },
  { id: "side", title: "Side by side", hint: "Statement · VS Code · Terminal", tree: () => splitOf("row", 34, leaf("instr"), splitOf("row", 55, leaf("code"), leaf("term"))) },
  { id: "left", title: "Statement over terminal", hint: "Left: statement over the terminal; right: VS Code", tree: () => splitOf("row", 42, splitOf("col", 60, leaf("instr"), leaf("term")), leaf("code")) },
  { id: "codeleft", title: "VS Code first", hint: "Left: VS Code; right: statement over the terminal", tree: () => splitOf("row", 55, leaf("code"), splitOf("col", 55, leaf("instr"), leaf("term"))) },
  { id: "top", title: "Statement on top", hint: "Statement above; VS Code and the terminal below", tree: () => splitOf("col", 38, leaf("instr"), splitOf("row", 55, leaf("code"), leaf("term"))) },
];
const CUSTOM = { id: "custom", title: "Custom", hint: "Drag a panel by its header, or a splitter, to arrange them yourself" };
const LAYOUT_KEY = "cellar.layout";

function treePanels(t) { return t && t.panel ? [t.panel] : t && t.a && t.b ? [...treePanels(t.a), ...treePanels(t.b)] : []; }
function treeValid(t) {
  const p = treePanels(t).sort().join();
  const ok = (n) => n.panel ? n.panel in PANELS : (n.dir === "row" || n.dir === "col") && typeof n.ratio === "number" && ok(n.a) && ok(n.b);
  return p === "code,instr,term" && ok(t);
}
function treeFind(root, pred, parent = null, key = null) {
  if (pred(root)) return { node: root, parent, key };
  return root.panel ? null : treeFind(root.a, pred, root, "a") || treeFind(root.b, pred, root, "b");
}
function treeRemove(root, panel) {   // the panel's sibling takes the place of their parent
  const loc = treeFind(root, (n) => n.panel === panel);
  if (!loc || !loc.parent) return root;
  const sibling = loc.parent[loc.key === "a" ? "b" : "a"];
  const up = treeFind(root, (n) => n === loc.parent);
  if (!up.parent) return sibling;
  up.parent[up.key] = sibling;
  return root;
}
function treeInsert(root, target, panel, edge) {   // dock `panel` against one edge of `target`
  const loc = treeFind(root, (n) => n.panel === target);
  const dir = edge === "left" || edge === "right" ? "row" : "col";
  const moved = leaf(panel), there = { ...loc.node };
  const node = edge === "left" || edge === "top" ? splitOf(dir, 50, moved, there) : splitOf(dir, 50, there, moved);
  if (!loc.parent) return node;
  loc.parent[loc.key] = node;
  return root;
}
function treeSwap(root, a, b) {
  const la = treeFind(root, (n) => n.panel === a), lb = treeFind(root, (n) => n.panel === b);
  la.node.panel = b; lb.node.panel = a;
  return root;
}
/** Percent rectangle of every panel, and the splitters (the line between the two halves of every split). */
function treeRects(node, rect, rects, splits) {
  if (node.panel) { rects[node.panel] = rect; return; }
  const row = node.dir === "row";
  const size = (row ? rect.w : rect.h) * node.ratio / 100;
  treeRects(node.a, row ? { ...rect, w: size } : { ...rect, h: size }, rects, splits);
  treeRects(node.b, row ? { ...rect, x: rect.x + size, w: rect.w - size } : { ...rect, y: rect.y + size, h: rect.h - size }, rects, splits);
  splits.push({ node, axis: row ? "x" : "y", rect, at: row ? rect.x + size : rect.y + size });
}
function layoutIcon(tree) {
  const rects = {};
  treeRects(tree, { x: 0, y: 0, w: 34, h: 24 }, rects, []);
  const svg = document.createElementNS("http://www.w3.org/2000/svg", "svg");
  svg.setAttribute("viewBox", "0 0 34 24"); svg.setAttribute("class", "lmicon");
  for (const [k, r] of Object.entries(rects)) {
    const el = document.createElementNS("http://www.w3.org/2000/svg", "rect");
    for (const [a, v] of Object.entries({ x: r.x + 0.5, y: r.y + 0.5, width: r.w - 1, height: r.h - 1, rx: 1, class: "lm-" + k })) el.setAttribute(a, v);
    svg.append(el);
  }
  return svg;
}

function mountDock(bodies) {
  const readJson = (key) => { try { return JSON.parse(localStorage.getItem(key)); } catch (e) { return null; } };
  const writeJson = (key, v) => { try { localStorage.setItem(key, JSON.stringify(v)); } catch (e) { /* private window: not remembered */ } };
  const clone = (t) => JSON.parse(JSON.stringify(t));
  const preset = (id) => LAYOUTS.find((l) => l.id === id);
  const saved = readJson(LAYOUT_KEY);
  let custom = readJson(LAYOUT_KEY + ".custom");
  if (!treeValid(custom)) custom = null;
  const keep = saved && (preset(saved.name) || saved.name === "custom") && treeValid(saved.tree);
  let name = keep ? saved.name : "default";
  let tree = keep ? saved.tree : preset("default").tree();
  const frames = [bodies.term, bodies.code];
  const guard = (on) => frames.forEach((f) => { f.style.pointerEvents = on ? "none" : ""; });

  const panels = {}, heads = {};
  for (const key of Object.keys(PANELS)) {
    heads[key] = h("div", { class: "dhead", title: "Drag to move this panel" }, h("span", { class: "grip" }, "⠿"), PANELS[key]);
    panels[key] = h("section", { class: "dpanel", "data-panel": key }, heads[key], h("div", { class: "dbody-wrap" }, bodies[key]));
  }
  const splitEls = [h("div", { class: "dsplit", title: "Drag to resize" }), h("div", { class: "dsplit", title: "Drag to resize" })];
  const hint = h("div", { class: "drophint" });
  const dock = h("div", { class: "dock" }, ...Object.values(panels), ...splitEls, hint);
  let splits = [];

  const persist = () => { writeJson(LAYOUT_KEY, { name, tree }); if (name === "custom") writeJson(LAYOUT_KEY + ".custom", tree); };
  const render = () => {
    const rects = {};
    splits = [];
    treeRects(tree, { x: 0, y: 0, w: 100, h: 100 }, rects, splits);
    for (const [k, r] of Object.entries(rects)) Object.assign(panels[k].style, { left: r.x + "%", top: r.y + "%", width: r.w + "%", height: r.h + "%" });
    splitEls.forEach((el, i) => {
      const s = splits[i];
      el.classList.toggle("h", s.axis === "y");
      Object.assign(el.style, s.axis === "x" ? { left: s.at + "%", top: s.rect.y + "%", height: s.rect.h + "%", width: "" } : { top: s.at + "%", left: s.rect.x + "%", width: s.rect.w + "%", height: "" });
    });
    drawMenu();
    persist();
  };
  const becomeCustom = () => { name = "custom"; custom = clone(tree); };

  // splitters
  splitEls.forEach((el, i) => {
    let stopDrag = null;
    el.addEventListener("pointerdown", (ev) => {
      ev.preventDefault();
      const s = splits[i];
      const box = dock.getBoundingClientRect();
      guard(true); el.classList.add("on"); document.body.style.cursor = s.axis === "x" ? "col-resize" : "row-resize";
      const move = (e) => {
        const p = s.axis === "x" ? 100 * (e.clientX - box.left) / box.width : 100 * (e.clientY - box.top) / box.height;
        const from = s.axis === "x" ? s.rect.x : s.rect.y, span = s.axis === "x" ? s.rect.w : s.rect.h;
        s.node.ratio = Math.min(85, Math.max(15, 100 * (p - from) / span));
        becomeCustom(); render();
      };
      stopDrag = () => { document.removeEventListener("pointermove", move); document.removeEventListener("pointerup", stopDrag); guard(false); el.classList.remove("on"); document.body.style.cursor = ""; stopDrag = null; };
      document.addEventListener("pointermove", move);
      document.addEventListener("pointerup", stopDrag);
    });
    el.stopDrag = () => stopDrag && stopDrag();
  });

  // dragging a panel by its header: dropping on the middle of another panel swaps them, on an edge docks it there
  const edgeOf = (r, x, y) => {
    const rx = (x - r.left) / r.width, ry = (y - r.top) / r.height;
    if (rx > 0.25 && rx < 0.75 && ry > 0.25 && ry < 0.75) return "center";
    const dist = { left: rx, right: 1 - rx, top: ry, bottom: 1 - ry };
    return Object.keys(dist).reduce((a, b) => (dist[a] < dist[b] ? a : b));
  };
  const showHint = (r, edge) => {
    const box = dock.getBoundingClientRect();
    let x = r.left - box.left, y = r.top - box.top, w = r.width, hh = r.height;
    if (edge === "left") w /= 2; else if (edge === "right") { x += w / 2; w /= 2; } else if (edge === "top") hh /= 2; else if (edge === "bottom") { y += hh / 2; hh /= 2; }
    Object.assign(hint.style, { left: x + "px", top: y + "px", width: w + "px", height: hh + "px", display: "block" });
  };
  let stopPanelDrag = null;
  for (const key of Object.keys(PANELS)) {
    heads[key].addEventListener("pointerdown", (ev) => {
      if (ev.button !== 0) return;
      ev.preventDefault();
      let target = null, edge = null;
      guard(true); document.body.style.cursor = "grabbing";
      const move = (e) => {
        target = null; hint.style.display = "none";
        for (const k of Object.keys(PANELS)) {
          if (k === key) continue;
          const r = panels[k].getBoundingClientRect();
          if (e.clientX >= r.left && e.clientX <= r.right && e.clientY >= r.top && e.clientY <= r.bottom) { target = k; edge = edgeOf(r, e.clientX, e.clientY); showHint(r, edge); break; }
        }
      };
      const up = () => {
        const t = target, e = edge;
        stopPanelDrag();
        if (!t) return;
        tree = clone(tree);
        if (e === "center") tree = treeSwap(tree, key, t);
        else tree = treeInsert(treeRemove(tree, key), t, key, e);
        becomeCustom(); render();
      };
      stopPanelDrag = () => { document.removeEventListener("pointermove", move); document.removeEventListener("pointerup", up); guard(false); document.body.style.cursor = ""; hint.style.display = "none"; stopPanelDrag = null; };
      document.addEventListener("pointermove", move);
      document.addEventListener("pointerup", up);
    });
  }

  // the Layout menu
  const menu = h("div", { class: "layoutmenu", role: "menu", hidden: true });
  const menuBtn = h("button", { class: "btn small", "aria-haspopup": "true", "aria-expanded": "false", title: "Choose how the statement, terminal and VS Code are arranged" }, "⬚ Layout");
  const choose = (id) => {
    if (id === "custom") { custom = custom || clone(tree); tree = clone(custom); name = "custom"; }
    else { tree = preset(id).tree(); name = id; }
    setMenu(false); render();
  };
  function drawMenu() {
    const row = (l, t) => h("button", { class: "lmitem" + (name === l.id ? " on" : ""), role: "menuitem", onclick: () => choose(l.id) }, layoutIcon(t), h("span", { class: "lmtext" }, h("b", {}, l.title), h("span", { class: "muted" }, l.hint)), h("span", { class: "lmcheck" }, name === l.id ? "✓" : ""));
    menu.replaceChildren(h("div", { class: "lmlabel muted" }, "Prebuilt"), ...LAYOUTS.map((l) => row(l, l.tree())), h("div", { class: "lmlabel muted" }, "Your own"), row(CUSTOM, custom || tree));
  }
  const setMenu = (open) => { menu.hidden = !open; menuBtn.setAttribute("aria-expanded", String(open)); };
  menuBtn.addEventListener("click", (ev) => { ev.stopPropagation(); setMenu(menu.hidden); });
  const outside = (ev) => { if (!menu.hidden && !menu.contains(ev.target)) setMenu(false); };
  const onEsc = (ev) => { if (ev.key === "Escape") setMenu(false); };
  document.addEventListener("click", outside);
  document.addEventListener("keydown", onEsc);

  const bar = h("div", { class: "dockbar" }, h("span", { class: "muted" }, "Drag a panel by its header to rearrange them"), h("span", { class: "spacer" }), h("div", { class: "layoutpick" }, menuBtn, menu));
  const el = h("div", { class: "dockwrap" }, bar, dock);
  render();
  return {
    el,
    destroy() {
      document.removeEventListener("click", outside); document.removeEventListener("keydown", onEsc);
      if (stopPanelDrag) stopPanelDrag();
      splitEls.forEach((s) => s.stopDrag());
    },
  };
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
  const list = codingModel(idx).order;
  const chapter = idx.chapters.find((c) => c.tag === d.tag && c.id === d.topic);
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
  const infoBtn = h("button", { class: "btn", title: "Reference entries involved in this exercise (which commands to look up, not how to use them)" }, "Recommended commands");
  infoBtn.addEventListener("click", () => {
    const old = $(".infobox", extras);
    if (old) return old.remove();
    extras.prepend(h("div", { class: "panel infobox" },
      h("h3", {}, "Recommended commands"),
      h("p", { class: "muted" }, "The commands and keywords involved in this exercise. They tell you what to look up, not how to use it."),
      d.recommended.length ? h("ul", { class: "reclist" }, d.recommended.map((e) => h("li", {},
        h("a", { href: "#/reference/" + e.id, target: "_blank", title: "Open in the Reference (new tab)" }, h("code", {}, e.title)),
        e.header ? h("span", { class: "muted" }, " " + e.header) : null,
        h("span", { class: "muted" }, " · " + e.category), h("div", { class: "sub", html: rmd(e.summary) })))) : h("p", { class: "muted" }, "Nothing to recommend for this one.")));
  });
  const solBtn = h("button", { class: "btn" }, "Answer");
  solBtn.addEventListener("click", async () => {
    const st = (await api("coding/" + id)).status;
    if (st !== "passed" && !confirm("Show the answer? The exercise will be marked “solution viewed” until you pass it.")) return;
    const r = await api("coding/" + id + "/solution", {});
    setStatus(r.status);
    const old = $(".solbox", extras);
    if (old) old.remove();
    extras.append(h("div", { class: "panel solbox" }, h("b", {}, "Answer (reference solution)"), h("pre", {}, h("code", {}, r.solution)),
      r.refs.length ? h("div", { class: "qlinks" }, h("span", { class: "muted" }, "Reference: "), r.refs.map((x) => [h("a", { href: "#/reference/" + x.id, target: "_blank", rel: "noopener" }, h("code", {}, x.title)), " "])) : null));
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
        if (r.log.trim()) nodes.push(h("details", { class: "case" }, h("summary", {}, h("span", { class: "mark", style: "color:var(--warn)" }, "!"), r.build ? "Build output" : "Compiler warnings (fix them)"), h("div", { class: "body" }, h("pre", { class: "compile-log" }, r.log))));
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

  const frameUrl = (port, query) => `${location.protocol}//${location.hostname}:${port}/?${query}`;
  const payload = encodeURIComponent(JSON.stringify([["openFile", "vscode-remote://" + d.file]]));
  const codeFrame = h("iframe", { src: frameUrl(state.cfg.vscode_port, `folder=${encodeURIComponent(d.workspace)}&payload=${payload}`), title: "VS Code", allow: "clipboard-read; clipboard-write" });
  const termFrame = h("iframe", { src: frameUrl(state.cfg.term_port, "arg=" + encodeURIComponent(d.file.replace(/\/[^/]*$/, ""))), title: "Terminal", allow: "clipboard-read; clipboard-write" });   // opens in the folder of answer.c
  const left = h("div", { class: "dbody statement" },
    h("div", { class: "crumbs" }, h("a", { href: "#/coding" }, "Coding"), " › ", d.track === "derusting" ? "C derusting" : "C exercises", " › ", chapter ? chapter.title : d.topic, " · ", h("span", { class: "tag", title: "Course topic " + T(d.tag) }, T(d.tag)), " ", stars(d.stars)),
    h("div", { html: md(d.statement) }),
    d.theory ? h("div", { class: "qlinks" }, h("span", { class: "muted" }, "Theory: "), h("a", { href: "#/theory/" + d.theory.id, target: "_blank", rel: "noopener" }, `${d.theory.title} (${d.theory.count} questions)`)) : null,
    h("div", { class: "muted", style: "font-size:13px" }, "Write it in ", d.files.map((f, i) => [i ? ", " : "", h("code", {}, f)]), " in VS Code. It saves by itself."),
    h("div", { class: "actions" }, checkBtn, hintBtn, infoBtn, solBtn, resetBtn, h("span", { class: "spacer" }), chip),
    hintBox, results, extras,
    h("div", { class: "qnav" }, prev ? h("a", { class: "btn small step prev", href: "#/coding/" + prev.id }, h("span", { class: "steplabel" }, "Previous"), "← " + prev.title) : null, h("span", { class: "spacer" }), next ? h("a", { class: "btn small step next", href: "#/coding/" + next.id }, h("span", { class: "steplabel" }, "Next"), next.title + " →") : null));
  const dock = mountDock({ instr: left, term: termFrame, code: codeFrame });
  mount(dock.el);
  state.cleanup = () => { document.removeEventListener("keydown", onKey); dock.destroy(); };
}
// ------------------------------------------------------------------ theory
async function viewTheoryList() {
  setNav("theory");
  const idx = await api("index");
  const holder = h("div", {});
  const bar = makeFilters(FILTER_KEYS.theory, { levels: uniqueSorted(idx.theory.flatMap((t) => t.qs.map((q) => q.d))), levelLabel: (n) => "★".repeat(n), levelTitle: (n) => n + (n === 1 ? " star" : " stars"), onChange: () => render() });
  let render = () => {
    const f = bar.f;
    // a set shows only the questions that match the difficulty filter, with progress counted on those
    const sets = idx.theory.filter((t) => !f.tags.length || f.tags.includes(t.tag)).map((t) => {
      const qs = t.qs.filter((q) => !f.levels.length || f.levels.includes(q.d));
      return { ...t, count: qs.length, answered: qs.filter((q) => q.a).length, correct: qs.filter((q) => q.c).length };
    }).filter((t) => t.count);
    const total = idx.theory.filter((t) => !f.tags.length || f.tags.includes(t.tag)).reduce((a, t) => a + t.count, 0);
    const shown = sets.reduce((a, t) => a + t.count, 0);
    holder.replaceChildren(
      f.levels.length ? h("p", { class: "muted", style: "margin:0 0 10px" }, `Showing ${shown} of ${total} questions.`) : null,
      sets.length ? h("div", { class: "list" }, sets.map((s) => {
        const p = s.count ? s.correct / s.count : 0;
        return h("a", { class: "row", href: "#/theory/" + s.id },
          h("div", { class: "main" }, h("div", { class: "title" }, s.title, " ", h("span", { class: "tag", title: "Course topic " + T(s.tag) }, T(s.tag))), h("div", { class: "sub" }, `${s.count} questions · ${s.types.join(", ")} · ${s.answered} answered`)),
          h("div", { class: "bar", style: "width:120px" }, h("span", { style: `width:${pct(p)}%` })), h("span", { class: "pct" }, pct(p) + "%"));
      })) : h("p", { class: "muted" }, "No question matches these filters."));
  };
  const pathBox = h("div", {});
  const drawPath = () => {
    const f = bar.f;
    const visible = idx.theory.filter((t) => !f.tags.length || f.tags.includes(t.tag));
    const qs = visible.flatMap((t) => t.qs.filter((q) => !f.levels.length || f.levels.includes(q.d)).map((q) => ({ ...q, set: t })));
    const done = qs.filter((q) => q.c).length;
    const next = qs.find((q) => !q.c);
    pathBox.replaceChildren(qs.length ? h("div", { class: "panel", style: "display:flex;gap:14px;align-items:center;flex-wrap:wrap" },
      h("div", { style: "flex:1;min-width:220px" }, h("b", {}, "Suggested path"), h("div", { class: "muted", style: "font-size:13px" }, `${done} of ${qs.length} questions answered correctly. The sets follow the coding chapters, easiest questions first.`),
        h("div", { class: "bar", style: "margin-top:6px" }, h("span", { style: `width:${(done / qs.length) * 100}%` }))),
      next ? h("a", { class: "btn primary", href: "#/theory/" + next.set.id }, (done ? "Continue: " : "Start: ") + next.set.title) : h("b", {}, "Everything correct ✔")) : null);
  };
  const baseRender = render;
  render = () => { baseRender(); drawPath(); };
  render();
  mount(page(h("h1", {}, "Theory"), h("p", { class: "lead" }, "Short questions with an explanation after every answer: choose, type, predict the output, match, sort or put in order."), pathBox, bar.el, holder));
}

const TYPE_LABEL = { single: "Single choice", multiple: "Multiple choice (select all that apply)", fill: "Fill in the blank", order: "Put in order", predict: "Predict the output", match: "Match the pairs", sort: "Sort into categories" };

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
    const blank = (n) => { const inp = h("input", { type: "text", autocomplete: "off", spellcheck: false, size: 12, oninput: onChange }); inputs[n] = inp; return inp; };
    const parts = (text, mono) => text.split(/\{\{(\d+)\}\}/).map((p, k) => (k % 2 ? blank(Number(p)) : mono ? document.createTextNode(p) : h("span", { html: ic(p) })));
    const codeHasBlanks = !!q.code && /\{\{\d+\}\}/.test(q.code);
    const promptEl = h("div", { class: "fill" }, parts(q.prompt, false));
    const codeEl = codeHasBlanks ? h("pre", { class: "fillcode" }, h("code", {}, parts(q.code, true))) : null;
    return {
      el: h("div", {}, promptEl, codeEl),
      get: () => inputs.map((i) => i.value),
      ready: () => inputs.every((i) => i.value.trim()),
      lock(r) {
        inputs.forEach((inp, i) => { inp.disabled = true; inp.classList.add(r.blanks_ok[i] ? "right" : "wrong"); });
        if (!r.correct) (codeEl || promptEl).after(h("div", { class: "muted", style: "margin:6px 0" }, "Answer: ", r.answer.map((a, i) => [i ? ", " : "", h("code", {}, a)])));
      },
      focus: () => inputs[0].focus(),
      promptInline: true,
      codeInline: codeHasBlanks,
    };
  }
  if (q.type === "predict") {
    const ta = h("textarea", { class: "predict", rows: 4, spellcheck: false, placeholder: "Type the output…", oninput: onChange });
    const el = h("div", {}, h("div", { class: "muted", style: "font-size:13px;margin:6px 0" }, "Type exactly what the program prints. Several lines are fine (Ctrl+Enter to check). Spaces at the end of a line and blank lines at the end are ignored."), ta);
    return {
      el,
      get: () => ta.value,
      ready: () => ta.value.trim().length > 0,
      lock(r) {
        ta.disabled = true;
        ta.classList.add(r.correct ? "right" : "wrong");
        if (!r.correct) el.append(h("div", { class: "muted", style: "margin:8px 0 2px" }, "The program prints:"), h("pre", {}, r.answer));
      },
      focus: () => ta.focus(),
    };
  }
  if (q.type === "match" || q.type === "sort") {
    const options = q.type === "match" ? q.options : q.categories;
    const selects = q.items.map(() => h("select", { onchange: onChange }, h("option", { value: "" }, q.type === "match" ? "— choose —" : "— category —"), options.map((o) => h("option", { value: o }, o))));
    const rows = q.items.map((it, i) => h("div", { class: "assign-row" }, h("span", { class: "assign-item", html: ic(it) }), selects[i]));
    return {
      el: h("div", { class: "assign" }, rows),
      get: () => selects.map((x) => x.value),
      ready: () => selects.every((x) => x.value),
      lock(r) {
        selects.forEach((x, i) => {
          x.disabled = true;
          rows[i].classList.add(r.items_ok[i] ? "right" : "wrong");
          if (!r.items_ok[i]) rows[i].append(h("span", { class: "note" }, "→ ", h("b", { html: ic(r.answer[i]) })));
        });
      },
      focus: () => selects[0].focus(),
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
  const levels = readFilters(FILTER_KEYS.theory).levels;   // the difficulty chips of the Theory page also choose the questions here
  const all = set.questions;
  set.questions = all.filter((q) => !levels.length || levels.includes(q.difficulty || 1));
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
      h("h1", {}, set.title, " ", h("span", { class: "tag", title: "Course topic " + T(set.tag) }, T(set.tag))),
      h("p", { class: "lead" }, `${set.questions.length} questions · ${answered} answered before`, levels.length ? ` · difficulty ${levels.map((n) => "★".repeat(n)).join(" ")} only (${all.length} in the set; change it on the Theory page)` : ""),
      set.practice ? h("p", {}, h("a", { href: `#/coding?chapter=${set.practice.tag}/${set.practice.chapter}` }, `Practise this chapter: ${set.practice.count} coding exercises →`)) : null,
      set.questions.length ? null : h("div", { class: "panel" }, "No question of this set matches the difficulty filter."),
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
          (r.hints || []).map((t) => h("div", { class: "hint" }, "💡 " + t)),
          h("div", { html: md(r.explain) }),
          r.refs && r.refs.length ? h("div", { class: "qlinks" }, h("span", { class: "muted" }, "Reference: "), r.refs.map((x) => [h("a", { href: "#/reference/" + x.id, target: "_blank", rel: "noopener" }, h("code", {}, x.title)), " "])) : null,
          r.practice ? h("div", { class: "qlinks" }, h("span", { class: "muted" }, "Practice: "), h("a", { href: `#/coding?chapter=${r.practice.tag}/${r.practice.chapter}`, target: "_blank", rel: "noopener" }, `${r.practice.title} (${r.practice.count} exercises)`)) : null));
        checkBtn.style.display = "none"; nextBtn.style.display = ""; nextBtn.focus();
      };
      checkBtn.addEventListener("click", submit);
      nextBtn.addEventListener("click", () => { i++; i < queue.length ? showQuestion() : summary(); });
      const onKey = (e) => {
        if (e.key !== "Enter" || e.target.tagName === "BUTTON" || e.shiftKey) return;
        if (e.target.tagName === "TEXTAREA" && !(e.ctrlKey || e.metaKey)) return;   // Enter is a new line in the output box
        e.preventDefault();
        done ? nextBtn.click() : submit();
      };
      document.addEventListener("keydown", onKey);
      const prompt = widget.promptInline ? null : h("div", { class: "qprompt", html: ic(q.prompt) });
      mount(page(
        h("div", { class: "crumbs" }, h("a", { href: "#/theory" }, "Theory"), " › ", h("a", { href: "#/theory/" + id }, set.title)),
        h("div", { class: "bar qprog" }, h("span", { style: `width:${(i / queue.length) * 100}%` })),
        h("div", { class: "qcard" },
          h("div", { class: "qmeta" }, h("span", {}, `Question ${i + 1} of ${queue.length}`), h("span", { class: "tag" }, TYPE_LABEL[q.type]), h("span", { class: "tag" }, q.topic), h("span", { class: "tag", title: "Course topic " + T(q.tag) }, T(q.tag)), stars(q.difficulty || 1)),
          prompt, q.code && !widget.codeInline ? h("pre", {}, h("code", {}, q.code)) : null, widget.el, feedback,
          h("div", { class: "qnav" }, checkBtn, nextBtn))));
      state.cleanup = () => document.removeEventListener("keydown", onKey);
      refresh();
      setTimeout(() => widget.focus && widget.focus(), 0);
    };
    const summary = () => {
      mount(page(h("div", { class: "center" },
        h("div", { class: "big" }, `${right}/${queue.length}`),
        h("p", { class: "lead" }, right === queue.length ? "Perfect round." : "Review what you missed, then try again."),
        missedNow.length ? h("div", { class: "list", style: "text-align:left;margin:16px 0" }, missedNow.map((q) => h("div", { class: "row" }, h("div", { class: "main" }, h("div", { class: "title" }, q.prompt.replace(/\{\{\d+\}\}/g, "____").replace(/`/g, "")), h("div", { class: "sub" }, TYPE_LABEL[q.type]))))) : null,
        h("p", {}, missedNow.length ? h("button", { class: "btn primary", onclick: () => run(missedNow.slice()) }, "Retry the missed ones") : null, " ",
          h("a", { class: "btn", href: "#/theory" }, "Back to theory"), " ", h("button", { class: "btn", onclick: () => viewTheorySet(id) }, "Restart")))));
    };
    showQuestion();
  };
  intro();
}

// ------------------------------------------------------------------ reference
const rn = (x) => String(x || "").toLowerCase();
const rmd = (t) => esc(t).replace(/`([^`]+)`/g, "<code>$1</code>");
function refPrep(e) {
  return { name: rn(e.title), aliases: (e.aliases || []).map(rn), flags: (e.details || []).map((d) => rn(d.name)),
    syntax: rn(e.syntax), summary: rn(e.summary), cat: rn(e.category), header: rn(e.header),
    body: rn([e.description, e.example, (e.mistakes || []).join(" "), (e.details || []).map((d) => d.text).join(" ")].join(" ")) };
}
/** How well one search word matches one entry: a name beats a prefix, which beats an option, a summary and, last, a mention in the text. */
function refScore(p, t) {
  let best = 0;
  const up = (v) => { if (v > best) best = v; };
  const long = t.length >= 3;
  if (p.name === t || p.aliases.includes(t)) up(1000);
  else if (p.name.startsWith(t)) up(800 - Math.min(99, p.name.length - t.length));
  else if (p.aliases.some((a) => a.startsWith(t))) up(600);
  else if (long && p.name.includes(t)) up(350);
  else if (long && p.aliases.some((a) => a.includes(t))) up(300);
  if (p.flags.some((f) => f.split(/[\s,|/]+/).includes(t))) up(450);       // one of its options, flags or formats: O_CREAT, %zu
  else if (long && p.flags.some((f) => f.includes(t))) up(250);
  if (long && p.syntax.includes(t)) up(260);
  if (long && p.header.includes(t)) up(120);
  if (long && p.summary.includes(t)) up(150);
  if (t.length >= 2 && p.cat.startsWith(t)) up(60);
  if (long && best < 150 && p.body.includes(t)) up(40);
  return best;
}
function refRank(entries, preps, query) {
  const terms = rn(query).split(/\s+/).filter(Boolean);
  const scored = [];
  for (const e of entries) {
    let sum = 0;
    for (const t of terms) { const v = refScore(preps.get(e.id), t); if (!v) { sum = 0; break; } sum += v; }
    if (sum) scored.push([e, sum]);
  }
  // when something matches by name or option, entries that merely mention the word in their text are noise
  const strong = scored.some(([, v]) => v >= 250 * terms.length);
  const out = strong ? scored.filter(([, v]) => v >= 120 * terms.length) : scored;
  out.sort((x, y) => y[1] - x[1] || x[0].title.length - y[0].title.length || x[0].title.localeCompare(y[0].title));
  return out.map(([e]) => e);
}


/** "Title" heading plus the first few rows; "and N more" opens the rest (and "show fewer" folds them again). Nothing when there are none. */
function moreList(title, data, row, shown = 5) {
  if (!data || !data.count || !title) return null;
  const rest = data.items.slice(shown);
  const ul = h("ul", {}, data.items.slice(0, shown).map(row));
  if (rest.length) {
    const li = h("li", { class: "muted" });
    const btn = h("button", { class: "linkbtn", "aria-expanded": "false" }, `and ${rest.length} more`);
    const extra = rest.map(row);
    let open = false;
    btn.addEventListener("click", () => {
      open = !open;
      extra.forEach((x) => (open ? ul.insertBefore(x, li) : x.remove()));
      btn.textContent = open ? "show fewer" : `and ${rest.length} more`;
      btn.setAttribute("aria-expanded", String(open));
    });
    li.append(btn);
    ul.append(li);
  }
  return [h("h5", {}, title), ul];
}

/** The star that marks a reference entry as a favorite. */
function favButton(on, toggle) {
  const b = h("button", { class: "favbtn" + (on ? " on" : ""), "aria-pressed": String(on), title: on ? "Remove from favorites" : "Add to favorites" }, on ? "★" : "☆");
  b.addEventListener("click", toggle);
  return b;
}

function entryCard(e, byId, fav) {
  return h("article", { class: "entry" },
    e.header ? h("span", { class: "hdr" }, e.header) : null,
    h("h3", {}, h("code", {}, e.title), " ", h("span", { class: "tag", title: "Course topic " + T(e.tag) }, T(e.tag)), " ", fav ? favButton(fav.on, fav.toggle) : null), h("div", { class: "cat" }, e.category, e.aliases && e.aliases.length ? ` · also: ${e.aliases.join(", ")}` : ""),
    h("p", { class: "esum", html: rmd(e.summary) }),
    e.syntax ? h("pre", {}, h("code", {}, e.syntax)) : null,
    e.description ? h("div", { html: e.description.split("\n").map((p) => "<p>" + rmd(p) + "</p>").join("") }) : null,
    e.details ? h("dl", {}, e.details.flatMap((d) => [h("dt", {}, d.name), h("dd", { html: rmd(d.text) })])) : null,
    e.example ? [h("h5", {}, "Example"), h("pre", {}, h("code", {}, e.example))] : null,
    e.mistakes ? [h("h5", {}, "Common mistakes"), h("ul", {}, e.mistakes.map((m) => h("li", { html: rmd(m) })))] : null,
    moreList(e.questions && e.questions.count ? `Theory questions (${e.questions.count})` : "", e.questions, (q) => h("li", {}, h("a", { href: "#/theory/" + q.set }, q.prompt.replace(/\{\{\d+\}\}/g, "____").replace(/`/g, "")))),
    moreList(e.practice && e.practice.count ? `Practice (${e.practice.count} exercise${e.practice.count === 1 ? "" : "s"} use it)` : "", e.practice && { count: e.practice.count, items: e.practice.exercises }, (x) => h("li", {}, h("a", { href: "#/coding/" + x.id }, x.title), " ", stars(x.stars))),
    e.see ? h("div", { class: "muted", style: "margin-top:10px" }, "See also: ", e.see.map((s) => { const t = byId.get(s); return t ? [h("a", { href: "#/reference/" + s }, h("code", {}, t.title)), " "] : null; })) : null);
}

async function viewReference(focusId) {
  setNav("reference");
  const loaded = await api("reference");
  const everything = loaded.entries;
  const favs = new Set(loaded.favorites);
  const readCollapsed = () => { try { return new Set(JSON.parse(localStorage.getItem(FILTER_KEYS.reference + ".collapsed") || "[]")); } catch (e) { return new Set(); } };
  const collapsed = readCollapsed();
  const saveCollapsed = () => { try { localStorage.setItem(FILTER_KEYS.reference + ".collapsed", JSON.stringify([...collapsed])); } catch (e) { /* private window: not remembered */ } };
  let category = null;   // a category chosen in the sidebar: only its entries are listed
  const preps = new Map(everything.map((e) => [e.id, refPrep(e)]));
  const byId = new Map(everything.map((e) => [e.id, e]));
  let all = everything, cats = [];
  const bar = makeFilters(FILTER_KEYS.reference, { onChange: () => refilter() });
  const recompute = () => {
    all = everything.filter((e) => !bar.f.tags.length || bar.f.tags.includes(e.tag));
    cats = [];
    for (const e of all) if (!cats.includes(e.category)) cats.push(e.category);
  };
  recompute();
  const search = h("input", { type: "search", placeholder: "Search a function or command: printf, open, dup2…", autofocus: true, autocomplete: "off", spellcheck: false });
  const nav = h("nav", { class: "refnav" });
  const pane = h("div", { class: "refpane" });
  let current = null, results = [], query = "";

  const deselect = () => { current = null; if (location.hash !== "#/reference") location.hash = "#/reference"; show(); };
  const link = (e, showCat) => {
    const a = h("a", { href: "#/reference/" + e.id, "data-id": e.id, class: "reflink" + (e.id === current ? " on" : "") }, h("code", {}, e.title, favs.has(e.id) ? h("span", { class: "favmark", title: "Favorite" }, " ★") : null), showCat ? h("span", { class: "muted rcat" }, e.category) : null);
    a.addEventListener("click", (ev) => { if (e.id === current) { ev.preventDefault(); deselect(); } });   // clicking the selected entry again closes it
    return a;
  };
  const byTitle = (x, y) => x.title.localeCompare(y.title);
  const setCategory = (c) => { category = c; if (c) collapsed.delete(c); saveCollapsed(); if (current && c && byId.get(current).category !== c) current = null; drawNav(); show(); };
  const toggleFav = async (e) => {
    const on = !favs.has(e.id);
    const r = await api("reference/favorite", { id: e.id, on });
    favs.clear(); r.favorites.forEach((i) => favs.add(i));
    drawNav(); show();
  };
  const catSection = (c, rows) => {
    const sec = h("section", { class: "refcat" + (collapsed.has(c) && !category ? " collapsed" : "") });
    const toggle = h("button", { class: "cattoggle", "aria-expanded": String(!sec.classList.contains("collapsed")), title: "Collapse or expand " + c, "aria-label": "Collapse or expand " + c }, "▾");
    toggle.addEventListener("click", () => {
      const now = sec.classList.toggle("collapsed");
      toggle.setAttribute("aria-expanded", String(!now));
      if (now) collapsed.add(c); else collapsed.delete(c);
      saveCollapsed();
    });
    const name = h("button", { class: "catname" + (category === c ? " on" : ""), title: category === c ? "Show all categories" : "Show only " + c }, c, " ", h("span", { class: "muted" }, String(rows.length)));
    name.addEventListener("click", () => setCategory(category === c ? null : c));
    sec.append(h("div", { class: "cathead" }, category ? null : toggle, name), h("div", { class: "catlinks" }, rows.map((e) => link(e, false))));
    return sec;
  };
  const drawNav = () => {
    if (query) nav.replaceChildren(...(results.length ? results.map((e) => link(e, true)) : [h("p", { class: "muted", style: "padding:6px 10px" }, "No entry matches.")]));
    else nav.replaceChildren(...(category ? [h("button", { class: "linkbtn back", onclick: () => setCategory(null) }, "← All categories")] : []), ...cats.filter((c) => !category || c === category).map((c) => catSection(c, all.filter((e) => e.category === c).sort(byTitle))));
  };
  const show = () => {
    nav.querySelectorAll(".reflink").forEach((a) => a.classList.toggle("on", a.dataset.id === current));
    const on = nav.querySelector(".reflink.on");
    if (on) {
      const sec = on.closest(".refcat.collapsed");
      if (sec) { sec.classList.remove("collapsed"); collapsed.delete(byId.get(current).category); saveCollapsed(); sec.querySelector(".cattoggle").setAttribute("aria-expanded", "true"); }
      on.scrollIntoView({ block: "nearest" });
    }
    const e = byId.get(current);
    if (e) { pane.replaceChildren(h("button", { class: "btn small backref", onclick: deselect }, "← Back to Reference"), entryCard(e, byId, { on: favs.has(e.id), toggle: () => toggleFav(e) })); app.scrollTop = 0; return; }
    const entryRows = (list) => h("div", { class: "list" }, list.map((x) => h("a", { class: "row", href: "#/reference/" + x.id },
      h("div", { class: "main" }, h("div", { class: "title" }, h("code", {}, x.title), favs.has(x.id) ? h("span", { class: "favmark", title: "Favorite" }, " ★") : null), h("div", { class: "sub", html: rmd(x.summary) })), h("span", { class: "tag", title: "Course topic " + T(x.tag) }, T(x.tag)))));
    if (category) {
      const inCat = all.filter((x) => x.category === category).sort(byTitle);
      pane.replaceChildren(h("div", { class: "welcome" }, h("h1", {}, category), h("p", { class: "lead" }, `${inCat.length} entries.`), entryRows(inCat)));
      return;
    }
    const popular = ["printf", "scanf", "fgets", "malloc", "strcmp", "fopen", "open", "read"].map((n) => all.find((e) => rn(e.title) === n)).filter(Boolean);
    const myFavs = all.filter((x) => favs.has(x.id)).sort(byTitle);
    pane.replaceChildren(h("div", { class: "welcome" }, h("h1", {}, "Reference"),
      h("p", { class: "lead" }, `${all.length} entries in ${cats.length} categories: one per function, command and keyword.`),
      h("p", {}, "Type a name on the left, pick one, or click a category to list only its entries. Popular: ", popular.map((e) => [h("a", { href: "#/reference/" + e.id }, h("code", {}, e.title)), " "])),
      h("h2", {}, "Favorites"),
      myFavs.length ? entryRows(myFavs) : h("p", { class: "muted" }, "Press ☆ on an entry to keep it here."),
      h("p", { class: "muted" }, "The search ranks the entry called exactly what you typed first, then names that start with it, then options and formats (try ", h("code", {}, "O_CREAT"), " or ", h("code", {}, "%zu"), "), and last text that merely mentions it.")));
  };
  const applyQuery = () => {
    query = search.value.trim();
    results = query ? refRank(all, preps, query) : [];
    if (query) current = results.length ? results[0].id : null;
    drawNav();
    show();
  };
  const refilter = () => { recompute(); if (current && !all.some((e) => e.id === current)) current = null; applyQuery(); };
  search.addEventListener("input", applyQuery);
  search.addEventListener("keydown", (ev) => {
    if (!query || !results.length) return;
    const i = results.findIndex((e) => e.id === current);
    if (ev.key === "ArrowDown" || ev.key === "ArrowUp") { ev.preventDefault(); current = results[Math.max(0, Math.min(results.length - 1, i + (ev.key === "ArrowDown" ? 1 : -1)))].id; show(); }
    if (ev.key === "Enter") { location.hash = "#/reference/" + current; }
  });
  state.refSelect = (id) => {
    if (id && byId.has(id)) { if (query && !results.some((e) => e.id === id)) { search.value = ""; query = ""; results = []; drawNav(); } current = id; if (category && byId.get(id).category !== category) { category = null; drawNav(); } } else current = null;
    show();
  };
  current = focusId && byId.has(focusId) ? focusId : null;
  drawNav();
  mount(h("div", { class: "refwrap" }, h("aside", { class: "refside" }, bar.el, search, nav), pane));
  state.cleanup = () => { state.refSelect = null; };
  show();
  search.focus();
  setTimeout(() => search.focus(), 60);
}

// ------------------------------------------------------------------ readiness
async function viewReadiness() {
  setNav("readiness");
  const tag = state.cfg.tags[0];
  const r = await api("readiness?tag=" + tag);
  const cls = (s) => (s >= 0.75 ? "" : s >= 0.4 ? "warn" : "bad");
  mount(page(h("h1", {}, "Readiness ", h("span", { class: "tag" }, T(tag))),
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
  const [path, query = ""] = location.hash.slice(1).split("?");
  const [, a, b] = path.split("/");
  state.query = new URLSearchParams(query);
  try {
    if (a === "reference" && state.refSelect) { state.refSelect(b); return; }
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

function applyTheme(mode) {
  if (mode === "light" || mode === "dark") document.documentElement.dataset.theme = mode;
  else delete document.documentElement.dataset.theme;
}
function initSettings() {
  const btn = $("#settings"), pop = $("#settings-pop");
  let mode = localStorage.getItem("cellar.theme") || "system";
  applyTheme(mode);
  const radios = ["system", "light", "dark"].map((m) => {
    const input = h("input", { type: "radio", name: "theme", value: m, checked: m === mode, onchange: () => { mode = m; localStorage.setItem("cellar.theme", m); applyTheme(m); } });
    return h("label", { class: "seg" }, input, h("span", {}, m[0].toUpperCase() + m.slice(1)));
  });
  pop.replaceChildren(h("div", { class: "pop-title" }, "Settings"), h("div", { class: "pop-row" }, h("span", { class: "muted" }, "Theme"), h("div", { class: "segs", role: "radiogroup", "aria-label": "Theme" }, radios)),
    h("div", { class: "pop-note muted" }, "The code editor follows your system theme."));
  const close = () => { pop.hidden = true; btn.setAttribute("aria-expanded", "false"); };
  btn.addEventListener("click", (e) => { e.stopPropagation(); pop.hidden = !pop.hidden; btn.setAttribute("aria-expanded", String(!pop.hidden)); });
  document.addEventListener("click", (e) => { if (!pop.hidden && !pop.contains(e.target)) close(); });
  document.addEventListener("keydown", (e) => { if (e.key === "Escape" && !pop.hidden) { close(); btn.focus(); } });
}

async function init() {
  initSettings();
  state.cfg = await api("config");
  window.addEventListener("hashchange", route);
  route();
}
init();
