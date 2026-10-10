// Runs inside VS Code and the terminal (they are other origins than the c-cellar page, so their keys never reach it).
// It forwards the two c-cellar shortcuts to the page, which does the work: Alt+Enter (compile & run) and Alt+X (give the keyboard back).
// Keep the matching rule identical to frameShortcut() in app/static/app.js.
(function () {
  if (window.parent === window) return;
  addEventListener("keydown", function (ev) {
    if (!ev.altKey || ev.ctrlKey || ev.metaKey || ev.shiftKey || (ev.getModifierState && ev.getModifierState("AltGraph"))) return;
    var action = ev.code === "Enter" || ev.code === "NumpadEnter" ? "run" : ev.code === "KeyX" ? "release" : null;
    if (!action) return;
    ev.preventDefault();
    ev.stopImmediatePropagation();
    if (!ev.repeat) parent.postMessage({ cellar: "key", action: action }, "*");
  }, true);
})();
