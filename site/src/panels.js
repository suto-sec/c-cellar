/* The two panes app.js shows next to the statement, as ordinary page elements instead of VS Code and ttyd frames:
 *   editorPanel(d, h)    a text editor for the answer (one tab per file), saved in this browser as you type
 *   terminalPanel(d, h)  the machine's console (xterm.js when it could be loaded, plain text otherwise)
 * app.js treats both like the frames it used to have (focus(), contentWindow.postMessage), so they carry stubs for those. */
(function (root) {
  "use strict";
  const XTERM_JS = "https://cdn.jsdelivr.net/npm/@xterm/xterm@5.5.0/lib/xterm.js";
  const XTERM_CSS = "https://cdn.jsdelivr.net/npm/@xterm/xterm@5.5.0/css/xterm.css";
  const dec = new TextDecoder();
  const frameLike = (el, focus) => {
    el.contentWindow = { postMessage() { /* nothing to tell: it is in this page */ } };
    el.focus = focus;
    return el;
  };
  const editors = new Map();   // exercise id -> {setValue} so that "Reset file" can empty the open editor

  function editorPanel(d, h) {
    const Store = root.CellarStore;
    let current = d.files[0];
    const area = h("textarea", { class: "site-editor", spellcheck: "false", autocapitalize: "off", autocomplete: "off", "aria-label": "Your code" });
    const tabs = h("div", { class: "site-tabs" });
    const load = () => { area.value = Store.getCode(d.id, current); };
    const drawTabs = () => tabs.replaceChildren(...d.files.map((n) => h("button", { class: "site-tab" + (n === current ? " on" : ""), onclick: () => { current = n; load(); drawTabs(); area.focus(); } }, n)));
    area.addEventListener("input", () => Store.setCode(d.id, current, area.value));
    area.addEventListener("keydown", (e) => {
      if (e.key === "Tab" && !e.ctrlKey && !e.altKey && !e.metaKey) {   // a Tab types spaces: it must not leave the editor
        e.preventDefault();
        const a = area.selectionStart, b = area.selectionEnd;
        area.setRangeText("    ", a, b, "end");
        Store.setCode(d.id, current, area.value);
      }
    });
    load();
    drawTabs();
    editors.set(d.id, { reload: load });
    const el = h("div", { class: "site-panel" }, d.files.length > 1 ? tabs : null, area,
      h("div", { class: "site-note" }, "Saved in this browser. ", h("a", { href: "#", onclick: (e) => { e.preventDefault(); exportProgress(); } }, "Export progress"), " · ", h("a", { href: "#", onclick: (e) => { e.preventDefault(); importProgress(); } }, "Import")));
    return frameLike(el, () => area.focus());
  }

  function terminalPanel(d, h) {
    const VM = root.CellarVM;
    const host = h("div", { class: "site-term" });
    const note = h("div", { class: "site-term-note" }, "Press Check or Compile: that starts a small Linux computer inside this page (the first time it takes a minute).");
    const el = h("div", { class: "site-panel" }, note, host);
    let term = null, off = [];
    const useXterm = () => typeof root.Terminal === "function";
    const start = () => {
      if (term) return;
      if (useXterm()) {
        term = new root.Terminal({ convertEol: false, fontSize: 13, cursorBlink: true, theme: { background: "#1e1e1e" } });
        term.open(host);
        term.onData((s) => VM.send(s));
        VM.replay((b) => term.write(b));
        off.push(VM.onOutput((b) => term.write(b)));
      } else {
        const pre = h("pre", { class: "site-term-plain", tabindex: "0" });
        host.append(pre);
        const strip = (t) => t.replace(/\x1b\[[0-9;?]*[ -/]*[@-~]/g, "").replace(/\r(?!\n)/g, "");
        VM.replay((b) => { pre.textContent += strip(dec.decode(b)); });
        off.push(VM.onOutput((b) => { pre.textContent += strip(dec.decode(b)); pre.scrollTop = pre.scrollHeight; }));
        pre.addEventListener("keydown", (e) => { if (e.key.length === 1) VM.send(e.key); else if (e.key === "Enter") VM.send("\n"); else if (e.key === "Backspace") VM.send("\x7f"); });
        term = { focus: () => pre.focus() };
      }
      off.push(VM.onStatus((msg) => { note.textContent = msg; note.hidden = !msg; }));
    };
    // xterm.js is loaded when the terminal is first needed; without it the plain console is used
    const ensureXterm = () => new Promise((resolve) => {
      if (useXterm()) return resolve();
      const css = document.createElement("link"); css.rel = "stylesheet"; css.href = XTERM_CSS; document.head.appendChild(css);
      const s = document.createElement("script"); s.src = XTERM_JS; s.onload = resolve; s.onerror = resolve; document.head.appendChild(s);
    });
    el.wake = async () => { await ensureXterm(); start(); };
    el.wake();
    return frameLike(el, () => { if (term && term.focus) term.focus(); });
  }

  async function compile(d, run) {
    const files = {};
    for (const n of d.files) files[n] = root.CellarStore.getCode(d.id, n);
    if (d.files.every((n) => !files[n].trim())) { alert(d.files.join(", ") + " is empty: write your program first."); return; }
    try {
      const ex = await (await fetch(`data/coding/${d.id}.json`)).json();   // (with the headers and the files it is linked with)
      await root.CellarVM.compile(ex, files, run);
    } catch (e) { alert(e.message); }
  }

  // progress travels as a file, like in dyaa-practice: export from one browser, import in another
  function exportProgress() {
    const blob = new Blob([JSON.stringify(root.CellarStore.exportAll())], { type: "application/json" });
    const a = document.createElement("a");
    a.href = URL.createObjectURL(blob);
    a.download = "c-cellar-progress.json";
    a.click();
    setTimeout(() => URL.revokeObjectURL(a.href), 1000);
  }
  function importProgress() {
    const input = document.createElement("input");
    input.type = "file"; input.accept = "application/json";
    input.onchange = async () => {
      try { root.CellarStore.importAll(JSON.parse(await input.files[0].text())); location.reload(); }
      catch (e) { alert("That file is not a c-cellar progress file."); }
    };
    input.click();
  }

  root.CELLAR_SITE = {
    editorPanel, terminalPanel, compile, exportProgress, importProgress,
    codeReset(id) { const e = editors.get(id); if (e) e.reload(); },
  };
})(typeof window !== "undefined" ? window : globalThis);
