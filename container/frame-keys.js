// Runs inside VS Code and the terminal (they are other origins than the c-cellar page, so their keys never reach it).
// It forwards the c-cellar shortcuts to the page, which does the work: Alt+Enter (compile & run), Alt+X (instructions panel), Alt+T (terminal)
// and Alt+V (VS Code). The page can also ask this frame to take the keyboard ({cellar: "focus"}).
// Keep the matching rule identical to frameShortcut() in app/static/app.js.
(function () {
  if (window.parent === window) return;
  var KEYS = { Enter: "run", NumpadEnter: "run", KeyX: "instructions", KeyT: "terminal", KeyV: "code" };
  addEventListener("keydown", function (ev) {
    if (!ev.altKey || ev.ctrlKey || ev.metaKey || ev.shiftKey || (ev.getModifierState && ev.getModifierState("AltGraph"))) return;
    var action = KEYS[ev.code];
    if (!action) return;
    ev.preventDefault();
    ev.stopImmediatePropagation();
    if (!ev.repeat) parent.postMessage({ cellar: "key", action: action }, "*");
  }, true);
  addEventListener("message", function (ev) {
    if (ev.source !== window.parent || !ev.data || ev.data.cellar !== "focus") return;
    if (window.term && window.term.focus) { window.term.focus(); return; }                       // the terminal (ttyd)
    var el = document.querySelector(".monaco-editor .native-edit-context") || document.querySelector(".monaco-editor textarea.inputarea") || document.querySelector(".monaco-workbench");   // VS Code (the editor if one is open)
    if (el) el.focus();
  });
})();
