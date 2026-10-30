/* A tiny tar (ustar) writer and reader: the browser and the VM exchange a whole job (sources, tests) and its results as one file each. */
(function (root) {
  "use strict";
  const enc = new TextEncoder();
  const dec = new TextDecoder();
  const bytes = (x) => (typeof x === "string" ? enc.encode(x) : x);

  function octal(n, len) {   // a number as zero padded octal ending in NUL
    return n.toString(8).padStart(len - 1, "0") + "\0";
  }
  function put(buf, off, text) {
    buf.set(enc.encode(text), off);
  }

  /** entries: [{name, data (string | Uint8Array), mode?}] -> Uint8Array. Names must be shorter than 100 bytes. Parent folders are created by tar -x. */
  function create(entries) {
    const parts = [];
    for (const e of entries) {
      const data = bytes(e.data);
      if (enc.encode(e.name).length >= 100) throw new Error("tar: name too long: " + e.name);
      const h = new Uint8Array(512);
      put(h, 0, e.name);
      put(h, 100, octal(e.mode || 0o644, 8));
      put(h, 108, octal(0, 8));
      put(h, 116, octal(0, 8));
      put(h, 124, octal(data.length, 12));
      put(h, 136, octal(0, 12));
      put(h, 148, "        ");   // the checksum counts these eight bytes as spaces
      h[156] = 0x30;   // '0': a regular file
      put(h, 257, "ustar\0");
      put(h, 263, "00");
      let sum = 0;
      for (let i = 0; i < 512; i++) sum += h[i];
      put(h, 148, sum.toString(8).padStart(6, "0") + "\0 ");
      parts.push(h, data, new Uint8Array((512 - (data.length % 512)) % 512));
    }
    parts.push(new Uint8Array(1024));   // two empty blocks end the archive
    const out = new Uint8Array(parts.reduce((a, p) => a + p.length, 0));
    let off = 0;
    for (const p of parts) { out.set(p, off); off += p.length; }
    return out;
  }

  const field = (buf, off, len) => { let end = off; while (end < off + len && buf[end] !== 0) end++; return dec.decode(buf.subarray(off, end)); };

  /** Uint8Array of a tar file -> {name: Uint8Array} for the regular files (GNU long names supported). */
  function parse(buf) {
    const files = {};
    let off = 0, longName = null;
    while (off + 512 <= buf.length) {
      if (buf.subarray(off, off + 512).every((b) => b === 0)) break;
      let name = field(buf, off, 100);
      const prefix = field(buf, off + 345, 155);
      if (prefix) name = prefix + "/" + name;
      const size = parseInt(field(buf, off + 124, 12).trim() || "0", 8);
      const type = String.fromCharCode(buf[off + 156] || 0x30);
      const body = buf.subarray(off + 512, off + 512 + size);
      if (type === "L") longName = dec.decode(body).replace(/\0+$/, "");
      else if (type === "0" || type === "\0") {
        files[(longName || name).replace(/^\.\//, "")] = body.slice();
        longName = null;
      }
      off += 512 + Math.ceil(size / 512) * 512;
    }
    return files;
  }

  const api = { create, parse };
  if (typeof module !== "undefined" && module.exports) module.exports = api;
  else root.CellarTar = api;
})(typeof window !== "undefined" ? window : globalThis);
