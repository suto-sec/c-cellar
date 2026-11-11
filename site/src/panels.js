/* The two panes app.js shows next to the statement, as ordinary page elements instead of VS Code and ttyd frames:
 *   editorPanel(d, h)    the code editor: Monaco (the editor of VS Code: C colours, line numbers, brackets, themes) loaded from a CDN; until it has loaded, or
 *                        when the CDN cannot be reached, a plain text box. One tab per file, saved in this browser as you type
 *   terminalPanel(d, h)  the machine's console (xterm.js when it could be loaded, plain text otherwise); the machine starts as soon as the page opens
 * app.js treats both like the frames it used to have (focus(), contentWindow.postMessage), so they carry stubs for those. */
(function (root) {
  "use strict";
  const XTERM_JS = "https://cdn.jsdelivr.net/npm/@xterm/xterm@5.5.0/lib/xterm.js";
  const XTERM_CSS = "https://cdn.jsdelivr.net/npm/@xterm/xterm@5.5.0/css/xterm.css";
  const MONACO = "https://cdn.jsdelivr.net/npm/monaco-editor@0.52.2/min";
  const dec = new TextDecoder();
  const frameLike = (el, focus) => {
    el.contentWindow = { postMessage() { /* nothing to tell: it is in this page */ } };
    el.focus = focus;
    return el;
  };
  const editors = new Map();   // exercise id -> {reload} so that "Reset file" can refresh the open editor

  // ------------------------------------------------------------------ loading the libraries (xterm first: Monaco's AMD loader would swallow xterm's UMD wrapper)
  let xtermPromise = null;
  function loadXterm() {
    if (typeof root.Terminal === "function") return Promise.resolve();
    if (!xtermPromise) {
      xtermPromise = new Promise((resolve) => {
        const css = document.createElement("link"); css.rel = "stylesheet"; css.href = XTERM_CSS; document.head.appendChild(css);
        const s = document.createElement("script"); s.src = XTERM_JS; s.onload = resolve; s.onerror = resolve; document.head.appendChild(s);
      });
    }
    return xtermPromise;
  }
  let monacoPromise = null;
  function loadMonaco() {
    if (root.monaco) return Promise.resolve(root.monaco);
    if (!monacoPromise) {
      monacoPromise = loadXterm().then(() => new Promise((resolve, reject) => {
        const s = document.createElement("script");
        s.src = MONACO + "/vs/loader.js";
        s.onerror = () => reject(new Error("Monaco could not be loaded"));
        s.onload = () => {
          // the editor's background threads come from the CDN too: a cross-origin worker must be started through a tiny same-origin-looking bootstrap
          root.MonacoEnvironment = { getWorkerUrl: () => "data:text/javascript;charset=utf-8," + encodeURIComponent(
            `self.MonacoEnvironment = { baseUrl: "${MONACO}/" }; importScripts("${MONACO}/vs/base/worker/workerMain.js");`) };
          root.require.config({ paths: { vs: MONACO + "/vs" } });
          root.require(["vs/editor/editor.main"], () => resolve(root.monaco), reject);
        };
        document.head.appendChild(s);
      }));
      monacoPromise.catch(() => { monacoPromise = null; });
    }
    return monacoPromise;
  }

  // ------------------------------------------------------------------ the editor
  const isDark = () => document.documentElement.dataset.theme === "dark"
    || (!document.documentElement.dataset.theme && root.matchMedia && root.matchMedia("(prefers-color-scheme: dark)").matches);
  const languageOf = (name) => (/\.(c|h)$/i.test(name) ? "c" : "plaintext");
  // Alt+Enter, Alt+X/T/V/M and Ctrl+Enter are the page's shortcuts: Monaco would keep them for itself (Ctrl+Enter inserts a line), so they are handed back
  const PAGE_KEYS = [
    ["Enter", "Enter", { ctrl: true }], ["Enter", "Enter", { alt: true }], ["KeyX", "x", { alt: true }], ["KeyT", "t", { alt: true }],
    ["KeyV", "v", { alt: true }], ["KeyM", "m", { alt: true }],
  ];
  let openEditor = null;   // the one editor on the page (the exercise page has one; leaving it replaces it)

  function editorPanel(d, h) {
    const Store = root.CellarStore;
    let current = d.files[0], ed = null, monaco = null;
    const area = h("textarea", { class: "site-editor", spellcheck: "false", autocapitalize: "off", autocomplete: "off", "aria-label": "Your code" });
    const mount = h("div", { class: "site-monaco", hidden: true });
    const tabs = h("div", { class: "site-tabs" });
    const modelOf = (n) => {
      const uri = monaco.Uri.parse(`file:///${d.id}/${n}`);
      return monaco.editor.getModel(uri) || monaco.editor.createModel(Store.getCode(d.id, n), languageOf(n), uri);
    };
    const load = () => {
      if (ed) { const m = modelOf(current); if (m.getValue() !== Store.getCode(d.id, current)) m.setValue(Store.getCode(d.id, current)); ed.setModel(m); }
      else area.value = Store.getCode(d.id, current);
    };
    const drawTabs = () => tabs.replaceChildren(...d.files.map((n) => h("button", { class: "site-tab" + (n === current ? " on" : ""), onclick: () => { current = n; load(); drawTabs(); focus(); } }, n)));
    const focus = () => { if (ed) ed.focus(); else area.focus(); };
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
    const el = h("div", { class: "site-panel" }, d.files.length > 1 ? tabs : null, area, mount,
      h("div", { class: "site-note" }, "Saved in this browser. ", h("a", { href: "#", onclick: (e) => { e.preventDefault(); exportProgress(); } }, "Export progress"), " · ", h("a", { href: "#", onclick: (e) => { e.preventDefault(); importProgress(); } }, "Import")));

    loadMonaco().then((m) => {
      if (!el.isConnected) return;   // the student already left this page
      monaco = m;
      if (openEditor) { try { openEditor.dispose(); } catch (e) { /* gone */ } }
      ed = openEditor = monaco.editor.create(mount, {
        model: modelOf(current), theme: isDark() ? "vs-dark" : "vs", automaticLayout: true, minimap: { enabled: false }, scrollBeyondLastLine: false,
        fontSize: 14, fontFamily: "ui-monospace, SFMono-Regular, Menlo, Consolas, 'DejaVu Sans Mono', monospace", tabSize: 4, insertSpaces: true,
        detectIndentation: false, padding: { top: 10 }, renderLineHighlight: "line", bracketPairColorization: { enabled: true }, "semanticHighlighting.enabled": false,
        ariaLabel: "Your code",
      });
      ed.onDidChangeModelContent(() => Store.setCode(d.id, current, ed.getValue()));
      for (const [code, key, mods] of PAGE_KEYS) {
        const mod = (mods.ctrl ? monaco.KeyMod.CtrlCmd : 0) | (mods.alt ? monaco.KeyMod.Alt : 0);
        ed.addCommand(mod | monaco.KeyCode[code === "Enter" ? "Enter" : code], () =>
          document.dispatchEvent(new KeyboardEvent("keydown", { code, key, ctrlKey: !!mods.ctrl, altKey: !!mods.alt, bubbles: true, cancelable: true })));
      }
      const retheme = () => monaco.editor.setTheme(isDark() ? "vs-dark" : "vs");
      new MutationObserver(retheme).observe(document.documentElement, { attributes: true, attributeFilter: ["data-theme"] });
      if (root.matchMedia) root.matchMedia("(prefers-color-scheme: dark)").addEventListener("change", retheme);
      area.hidden = true;
      mount.hidden = false;
      const hadFocus = document.activeElement === area;
      if (hadFocus) ed.focus();
    }).catch(() => { /* no CDN: the text box stays */ });
    return frameLike(el, focus);
  }

  // ------------------------------------------------------------------ the terminal
  function terminalPanel(d, h) {
    const VM = root.CellarVM;
    const host = h("div", { class: "site-term" });
    const note = h("div", { class: "site-term-note" }, "Starting a small Linux computer inside this page (the first time it takes a minute)…");
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
    el.wake = async () => { await loadXterm(); start(); };
    // the machine starts now, not at the first Check: the terminal shows a prompt when the page has opened, and the first Check does not wait for the boot
    el.wake().then(() => VM.boot()).then(() => { note.hidden = true; }, (e) => { note.textContent = "The machine could not start: " + e.message; });
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
