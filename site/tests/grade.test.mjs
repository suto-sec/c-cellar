// The browser's grading (src/api.js) must say what app/server.py says. Vectors: python3 tests/make_grade_vectors.py > vectors.json
import fs from "node:fs";
import { createRequire } from "node:module";
const require = createRequire(import.meta.url);
const A = require("../src/api.js");
const vectors = JSON.parse(fs.readFileSync(process.argv[2], "utf8"));
let bad = 0;
for (const v of vectors) {
  const q = v.q;
  const got = A.grade(q, v.response);
  const problems = [];
  if (got !== v.grade) problems.push(`grade ${got} != ${v.grade}`);
  if (q.type === "fill" && JSON.stringify(A.blanksOk(q, v.response)) !== JSON.stringify(v.blanks_ok)) problems.push("blanks_ok differs");
  if ((q.type === "match" || q.type === "sort") && JSON.stringify(A.pairsOk(q, v.response)) !== JSON.stringify(v.items_ok)) problems.push("items_ok differs");
  if (q.type === "predict" && !got && JSON.stringify(A.predictHints(q, v.response)) !== JSON.stringify(v.hints)) problems.push(`hints ${JSON.stringify(A.predictHints(q, v.response))} != ${JSON.stringify(v.hints)}`);
  if (problems.length) { bad++; if (bad <= 10) console.log(q.id, JSON.stringify(v.response).slice(0, 80), problems.join("; ")); }
}
console.log(`${vectors.length} responses, ${bad} differences from the server's grading`);
process.exit(bad ? 1 : 0);
