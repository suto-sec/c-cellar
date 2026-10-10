// Runs inside VS Code only. A folder opens with the explorer visible. The first time this folder is opened here, close it; after that VS Code
// remembers whatever you do with it (open it again with Ctrl+B and it stays open).
(function () {
  var folder = new URLSearchParams(location.search).get("folder") || "";
  var key = "cellar.explorer.closed:" + folder;
  try { if (localStorage.getItem(key)) return; } catch (e) { return; }
  var tries = 0;
  var timer = setInterval(function () {
    var side = document.querySelector(".part.sidebar");
    var current = document.querySelector(".part.activitybar .action-item.checked .action-label");   // the icon of the view that is showing
    var open = !!side && side.offsetWidth > 0 && !!current;
    if (!open && ++tries < 150) return;                                     // wait for the workbench (up to 30 s)
    clearInterval(timer);
    try { localStorage.setItem(key, "1"); } catch (e) { /* private window */ }
    if (open) current.click();                                              // clicking the icon of the open view closes the side bar
  }, 200);
})();
