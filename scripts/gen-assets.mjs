#!/usr/bin/env node
/**
 * 单一数据源生成器
 * 输入：docs/data/progress.json
 * 输出：docs/data/progress.js（页面读取）+ 更新 index.html 里的版本号
 *
 * 用法：node scripts/gen-assets.mjs（在仓库根目录执行）
 */
import fs from "node:fs";
import { createHash } from "node:crypto";

const ROOT = process.cwd();
const read = p => fs.readFileSync(`${ROOT}/${p}`, "utf8");
const write = (p, s) => fs.writeFileSync(`${ROOT}/${p}`, s, "utf8");

const progress = JSON.parse(read("docs/data/progress.json"));

// 1) progress.js —— 页面优先加载这份
write("docs/data/progress.js", "window.PROGRESS = " + JSON.stringify(progress, null, 2) + ";\n");

// 2) 给 index.html 里的 progress.js 引用加内容版本号（防浏览器缓存旧数据）
{
  const js = read("docs/data/progress.js");
  const ver = createHash("sha256").update(js).digest("hex").slice(0, 8);
  let html = read("docs/index.html");
  html = html.replace(/<script src="data\/progress\.js(\?v=[^"]*)?"><\/script>/,
    `<script src="data/progress.js?v=${ver}"></script>`);
  write("docs/index.html", html);
}

console.log("generated docs/data/progress.js + versioned index.html");
