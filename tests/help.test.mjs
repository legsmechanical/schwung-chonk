/* help.json is written for a 128x64 screen with a ~20 character line width.
 * There is nothing at runtime that complains about a long line — it simply
 * runs off the display — so the check lives here.
 *
 *   node tests/help.test.mjs        (run by scripts/test.sh)
 */
import { readFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { fileURLToPath } from "node:url";

const repo = join(dirname(fileURLToPath(import.meta.url)), "..");
const MAX = 22;   /* the fleet's own practical ceiling; the doc says ~20 */

const help = JSON.parse(readFileSync(join(repo, "src", "help.json"), "utf8"));
const wrapper = readFileSync(join(repo, "src", "dsp", "chonk_plugin.cpp"), "utf8");

let fails = 0, lines = 0, leaves = 0;
const fail = (m) => { console.log(`  FAIL  ${m}`); fails++; };

function walk(node, path) {
  if (!node.title) fail(`node at ${path} has no title`);
  const here = `${path}/${node.title}`;
  const isBranch = Array.isArray(node.children);
  const isLeaf = Array.isArray(node.lines);
  if (isBranch === isLeaf) fail(`${here} must have exactly one of children/lines`);
  if (isLeaf) {
    leaves++;
    node.lines.forEach((l, i) => {
      lines++;
      if (typeof l !== "string") fail(`${here} line ${i} is not a string`);
      else if (l.length > MAX) fail(`${here} line ${i} is ${l.length} chars: "${l}"`);
    });
  }
  if (isBranch) node.children.forEach((c) => walk(c, here));
}
walk(help, "");

/* Every parameter the module serves should be findable in the help text —
 * a param nobody can look up is a param nobody knows is there. Matched on the
 * display NAME from PARAMS[], which is what the device shows. */
const block = wrapper.match(/static const param_def_t PARAMS\[\] = \{([\s\S]*?)\n\};/)[1];
const names = [...block.matchAll(/\{\s*"[^"]+"\s*,\s*"([^"]+)"/g)].map((m) => m[1]);
const text = JSON.stringify(help);
const missing = names.filter((n) => !text.includes(n.split(" ")[0]));
if (missing.length) fail(`help never mentions: ${missing.join(", ")}`);

if (!fails) {
  console.log(`  ok    help.json: ${leaves} topics, ${lines} lines, all <= ${MAX} chars`);
  console.log(`  ok    every parameter appears in the help text`);
}
console.log(`\nhelp: ${fails} failed`);
process.exit(fails ? 1 : 0);
