// Runs inside the terminal page only. The c-cellar page asks it, by message, to type a line in the terminal (the Compile buttons).
// Only the c-cellar page itself may ask: the message must come from the page that frames this one, and from one of the origins in ORIGINS
// (filled in by ttyd-index.py), so no other website that frames the terminal can type commands in it.
(function () {
  var ORIGINS = __ORIGINS__;
  addEventListener("message", function (ev) {
    if (ev.source !== window.parent || ORIGINS.indexOf(ev.origin) < 0) return;
    var d = ev.data;
    if (!d || d.cellar !== "type" || typeof d.text !== "string" || d.text.length > 4000 || d.text.indexOf("\n") >= 0 || !window.term) return;
    // (paste() because every ttyd version has it: the older terminal object of ttyd 1.7.4 has no input(). term.rc turns bracketed paste
    // off, so the final \r is typed like the Enter key, not pasted as text.)
    window.term.focus();
    window.term.paste("\x03");   // Ctrl+C: ends a program that is still running and drops a half-typed line
    setTimeout(function () { window.term.paste(d.text + "\r"); }, 150);
  });
})();
