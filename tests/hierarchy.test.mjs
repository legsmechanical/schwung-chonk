/* The page layout, checked against the parameter table it is drawn from.
 *
 *   node tests/hierarchy.test.mjs        (run by scripts/test.sh)
 *
 * None of this fails loudly on the device: a param with no home is simply
 * unreachable in the menus, a knob naming a key that does not exist is a dead
 * encoder, and two levels with the SAME authored knob signature get deduped by
 * the planner so the second one renders as an EMPTY page.
 */
import { readFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { fileURLToPath } from "node:url";

const repo = join(dirname(fileURLToPath(import.meta.url)), "..");
const mod = JSON.parse(readFileSync(join(repo, "src", "module.json"), "utf8"));
const wrapper = readFileSync(join(repo, "src", "dsp", "chonk_plugin.cpp"), "utf8");

const block = wrapper.match(/static const param_def_t PARAMS\[\] = \{([\s\S]*?)\n\};/)[1];
const keys = [...block.matchAll(/\{\s*"([^"]+)"\s*,\s*"[^"]*"\s*,\s*(?:"[^"]*"|NULL)\s*,\s*K_/g)]
  .map((m) => m[1]);

const levels = mod.ui_hierarchy?.levels || {};
let fails = 0;
const fail = (m) => { console.log(`  FAIL  ${m}`); fails++; };

/* 1. every knob and param names a real parameter or a level link */
const homes = new Map();
for (const [name, lvl] of Object.entries(levels)) {
  for (const k of lvl.knobs || [])
    if (!keys.includes(k)) fail(`level "${name}" maps knob to unknown key "${k}"`);
  for (const p of lvl.params || []) {
    if (typeof p !== "string") {
      if (!p.level) fail(`level "${name}" has a param entry with no key or level`);
      else if (!levels[p.level]) fail(`level "${name}" links to missing level "${p.level}"`);
      continue;
    }
    if (!keys.includes(p)) fail(`level "${name}" lists unknown param "${p}"`);
    if (homes.has(p)) fail(`param "${p}" appears on both "${homes.get(p)}" and "${name}"`);
    homes.set(p, name);
  }
}

/* 2. every parameter is reachable from some page */
for (const k of keys)
  if (!homes.has(k)) fail(`param "${k}" has no home page — unreachable in the menus`);

/* 3. no level repeats another's knob signature (the planner would dedupe it
 *    and render the second as an empty page) */
const sigs = new Map();
for (const [name, lvl] of Object.entries(levels)) {
  const sig = JSON.stringify(lvl.knobs || []);
  if (sigs.has(sig)) fail(`level "${name}" has the same knob signature as "${sigs.get(sig)}"`);
  sigs.set(sig, name);
  if ((lvl.knobs || []).length > 8) fail(`level "${name}" maps ${lvl.knobs.length} knobs; there are 8`);
}

/* 4. every level is reachable from root */
const seen = new Set(["root"]);
const walk = (n) => {
  for (const p of levels[n]?.params || [])
    if (typeof p === "object" && p.level && !seen.has(p.level)) { seen.add(p.level); walk(p.level); }
};
walk("root");
for (const name of Object.keys(levels))
  if (!seen.has(name)) fail(`level "${name}" is not reachable from root`);

if (!fails) {
  const shape = Object.entries(levels)
    .map(([n, l]) => `${n}(${(l.knobs || []).length})`).join(" ");
  console.log(`  ok    hierarchy: ${shape}`);
  console.log(`  ok    all ${keys.length} params have exactly one home page`);
}
console.log(`\nhierarchy: ${fails} failed`);
process.exit(fails ? 1 : 0);
