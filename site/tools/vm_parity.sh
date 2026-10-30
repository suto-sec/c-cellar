#!/bin/bash
# The full parity run: every reference solution through the real machine, in N parallel copies of it (each needs ~600 MB of RAM and one core).
#   bash site/tools/vm_parity.sh 4          # 4 shards; reports: site/vm_parity_report.<i>of<N>.json ; summary at the end
#   bash site/tools/vm_parity.sh 4 t4-      # only the exercises whose id contains "t4-"
N=${1:-3}; ONLY=${2:-}
cd "$(dirname "$0")/.."
rm -f vm_parity_report.*of*.json
for i in $(seq 0 $((N - 1))); do
  node tools/vm_smoke.mjs --all --only "$ONLY" --shard "$i/$N" > "vm_parity_$i.log" 2>&1 &
done
wait
cat vm_parity_*.log | grep -E "^(FAIL|[0-9]+ exercises)" | sort | head -200
node -e '
const fs = require("fs");
const all = fs.readdirSync(".").filter(f => /^vm_parity_report\..*of.*\.json$/.test(f)).flatMap(f => JSON.parse(fs.readFileSync(f)));
const bad = all.filter(r => !r.ok);
console.log(`\n${all.length} exercises through the machine, ${all.length - bad.length} pass, ${bad.length} differ; mean ${(all.reduce((a, r) => a + r.seconds, 0) / all.length).toFixed(1)}s each`);
fs.writeFileSync("vm_parity_report.json", JSON.stringify(all, null, 1));
'
