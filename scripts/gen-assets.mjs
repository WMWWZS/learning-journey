#!/usr/bin/env node
/**
 * 单一数据源生成器
 * 输入：docs/data/progress.json
 * 输出：docs/data/progress.js（页面读取）+ assets/dashboard.svg（README 用图）
 *
 * 用法：node scripts/gen-assets.mjs（在仓库根目录执行）
 */
import fs from "node:fs";

const ROOT = process.cwd();
const read = p => fs.readFileSync(`${ROOT}/${p}`, "utf8");
const write = (p, s) => fs.writeFileSync(`${ROOT}/${p}`, s, "utf8");

const progress = JSON.parse(read("docs/data/progress.json"));

// 1) progress.js —— 页面优先加载这份
write("docs/data/progress.js", "window.PROGRESS = " + JSON.stringify(progress, null, 2) + ";\n");

// 2) dashboard.svg —— 用模板填值
let svg = read("assets/dashboard.template.svg");
const KEY = { "C++": "CPP", "Qt": "QT", "OpenCV": "OPENCV", "Linux": "LINUX" };
const key = name => KEY[name] || name.toUpperCase().replace(/[^A-Z0-9]/g, "_");

for (const skill of progress.skills) {
  const k = key(skill.name);
  const w = Math.max(0, Math.min(400, Math.round((skill.pct / 100) * 400)));
  const cx = 196 + w;
  svg = svg
    .replaceAll(`{{P_${k}}}`, String(skill.pct))
    .replaceAll(`{{W_${k}}}`, String(w))
    .replaceAll(`{{CX_${k}}}`, String(cx));
}

const stage = progress.stages.find(s => s.state === "now") || progress.stages[0] || {};
const stageLabel = stage.short || (stage.items || []).join(" · ");
const stageLabelFit = stageLabel.length > 24 ? stageLabel.slice(0, 23) + "…" : stageLabel;
svg = svg
  .replaceAll("{{STAGE_NAME}}", stage.name || "—")
  .replaceAll("{{STAGE_WHEN}}", stage.when || "—")
  .replaceAll("{{STAGE_ITEMS}}", stageLabelFit)
  .replaceAll("{{TARGET_YEAR}}", String(progress.targetYear || "2027"));

const left = svg.match(/\{\{[A-Z0-9_]+\}\}/g);
if (left && left.length) {
  console.error("未替换的占位符：", left);
  process.exit(1);
}

write("assets/dashboard.svg", svg);

// 3) README 里的"当前进度"一行（放在 PROGRESS 标记之间）
let readme = read("README.md");
const progressLine =
  "**当前进度**：" +
  progress.skills.map(s => `${s.name} ${s.pct}%`).join(" · ") +
  " · [看详情 →](https://wmwwzs.github.io/learning-journey/#skills)";
readme = readme.replace(
  /<!-- PROGRESS:BEGIN -->[\s\S]*?<!-- PROGRESS:END -->/,
  "<!-- PROGRESS:BEGIN -->\n" + progressLine + "\n<!-- PROGRESS:END -->",
);
write("README.md", readme);

console.log("generated docs/data/progress.js + assets/dashboard.svg + README progress line");
