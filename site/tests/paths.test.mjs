// src/api.js against app/server.py on imported paths: the same verdict (the cleaned path or the message) for every vector of make_path_vectors.py,
// then the import / replace / delete flow through the api (what the page does).
import { createRequire } from "node:module";
import fs from "node:fs";
const require = createRequire(import.meta.url);
const vec = JSON.parse(fs.readFileSync(process.argv[2], "utf8"));
const store = new Map();
globalThis.localStorage = { getItem: (k) => (store.has(k) ? store.get(k) : null), setItem: (k, v) => store.set(k, String(v)), removeItem: (k) => store.delete(k), get length() { return store.size; }, key: (i) => [...store.keys()][i] };
const { normalizeCustomPath } = require("../src/api.js");
let bad = 0;
const eq = (name, a, b) => { if (JSON.stringify(a) !== JSON.stringify(b)) { bad++; console.log("DIFF", name, JSON.stringify(a), "|", JSON.stringify(b)); } };
const coding = new Set(vec.coding), chapters = new Set(vec.chapters), taken = new Set(vec.taken);
for (const [n, c] of vec.cases.entries()) {
  let mine;
  try { const r = normalizeCustomPath(c.in, coding, chapters, taken); mine = { path: r.path, warnings: r.warnings }; } catch (e) { mine = { error: e.message }; }
  const want = c.error !== undefined ? { error: c.error } : { path: c.path, warnings: c.warnings };
  eq("case " + n, mine, want);
}

// the flow, through the api with the real index
globalThis.fetch = async (p) => ({ ok: true, json: async () => ({ coding: vec.coding.map((id) => ({ id, tag: id.split("-")[0], status: "todo" })), theory: [], chapters: vec.chapters.map((k) => ({ tag: k.split("/")[0], id: k.split("/")[1] })), learning_paths: { t4: [{ id: "essentials" }] }, paths: {}, reference: 0 }) });
require("../src/api.js");
const api = globalThis.CELLAR_API;
const file = { id: "mine", title: "Mine", phases: [{ title: "A", note: "n", items: ["t4-procs-fork-hello", "zzz"], theory: ["t4/procs"] }] };
let r = await api("paths/import", { path: file });
eq("import", [r.ok, r.id, r.count, r.replaced, r.warnings.length], [true, "mine", 1, false, 1]);
let idx = await api("index");
eq("index.custom_paths", idx.custom_paths.map((p) => [p.id, p.items, p.theory]), [["mine", ["t4-procs-fork-hello"], ["t4/procs"]]]);
eq("source is the imported file", idx.custom_paths[0].source, file);
r = await api("paths/import", { path: { ...file, title: "Mine 2" } });
eq("replace", [r.replaced], [true]);
idx = await api("index");
eq("one path after replace", idx.custom_paths.map((p) => p.title), ["Mine 2"]);
for (const bad_ of [{ id: "essentials", title: "x", items: ["t4-procs-fork-hello"] }, { id: "course", title: "x", items: ["t4-procs-fork-hello"] }]) {
  let msg = ""; try { await api("paths/import", { path: bad_ }); } catch (e) { msg = e.message; }
  eq("built-in id refused: " + bad_.id, /built-in path/.test(msg), true);
}
const exp = JSON.parse(JSON.stringify(globalThis.CellarStore.exportAll()));   // (what the file holds)
await api("paths/delete", { id: "mine" });
idx = await api("index");
eq("deleted", idx.custom_paths.length, 0);
globalThis.CellarStore.importAll(exp);
idx = await api("index");
eq("restored from an export", idx.custom_paths.length, 1);
let msg = ""; try { await api("paths/delete", { id: "nope" }); } catch (e) { msg = e.message; }
eq("delete unknown", msg, "there is no imported path with that id");
console.log(bad ? `${bad} DIFFERENCES` : `paths: ${vec.cases.length} vectors and the flow: no differences`);
process.exit(bad ? 1 : 0);
