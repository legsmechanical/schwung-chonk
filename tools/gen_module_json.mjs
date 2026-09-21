/* Inject the wrapper's ui_hierarchy into src/module.json.
 *
 *   node tools/gen_module_json.mjs        (run by scripts/build.sh)
 *
 * WHY: the host fills a slot's param table from module.json ON DISK at load
 * (chain_host.c -> parse_chain_params), and the custom-knob write path strcmp's
 * against that table and silently does nothing on a miss. Without the inline
 * block the table is empty at load and slot custom knobs only work when
 * something else happens to refresh the cache — i.e. intermittently.
 *
 * Generated, not hand-copied: kUiHierarchy in the wrapper is the single source
 * of truth, and a duplicate would drift the moment a level is renamed.
 */
import { readFileSync, writeFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { fileURLToPath } from "node:url";

const here = dirname(fileURLToPath(import.meta.url));
const repo = join(here, "..");
const src = readFileSync(join(repo, "src", "dsp", "chonk_plugin.cpp"), "utf8");

const m = src.match(/static const char \*kUiHierarchy\s*=\s*([\s\S]*?);\s*\n/);
if (!m) { console.error("could not find kUiHierarchy in the wrapper"); process.exit(1); }

/* Strip C comments FIRST. The declaration is annotated, and a comment that
 * happens to quote a level name ("String") would otherwise be pulled in as
 * part of the JSON — which it was, and the only symptom was a parse error
 * pointing at a column that meant nothing. */
const literals = m[1].replace(/\/\*[\s\S]*?\*\//g, "").replace(/\/\/[^\n]*/g, "");

let out = "";
for (const lit of literals.matchAll(/"((?:[^"\\]|\\.)*)"/g))
  out += lit[1].replace(/\\"/g, '"').replace(/\\\\/g, "\\").replace(/\\n/g, "\n");

let uih;
try { uih = JSON.parse(out); }
catch (e) { console.error("wrapper ui_hierarchy is not valid JSON:", e.message); process.exit(1); }

const jsonPath = join(repo, "src", "module.json");
const mod = JSON.parse(readFileSync(jsonPath, "utf8"));
mod.ui_hierarchy = uih;
writeFileSync(jsonPath, JSON.stringify(mod, null, 2) + "\n");

const levels = Object.keys(uih.levels || {});
console.log(`module.json: embedded ui_hierarchy — ${levels.length} levels (${levels.join(", ")})`);
