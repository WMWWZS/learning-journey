# 🗂 仓库结构说明

> 每个文件是干嘛的、要不要手动改。**⚙️ = 自动生成，别手改；✅ = 你只需要改这些。**

## 一张图看懂

```text
learning-journey/
│
├── .github/workflows/          # GitHub Actions 自动化
│   ├── snake.yml               # ⚙️ 每天生成贡献蛇
│   ├── sync-commits.yml        # ⚙️ 每天同步提交数据
│   └── gen-assets.yml          # ⚙️ progress.json 变动后重新生成页面数据、README 图、README 进度行
│
├── archive/                    # 2026 上半年旧练习（29 个工程，只保留源码）
│   ├── cpp-homework/           #   C++ 作业（Cplus_homework_1~6、QThomeWork 7/8/16~19）
│   ├── qt-practice/            #   Qt 练习（Qt1~Qt5、Qt11~13、Qt18~20）
│   └── qt-demo/                #   Qt 小 demo（QT_demo_1~6）
│
├── assets/                     # README 里用的图（GitHub 直接渲染 SVG）
│   ├── eruption.svg            # ✅ README 顶部火山横幅（手绘 SVG）
│   ├── dashboard.svg           # ⚙️ 技能温度图（由 progress.json 生成）
│   ├── dashboard.template.svg  #    技能温度图模板（占位符，一般不动）
│   ├── roadmap.svg             # ✅ README 路线图
│   └── footer.svg              # ✅ README 页脚
│
├── cpp/                        # ✅ C++ 练习代码（按 年-月/日期-主题 建子目录）
├── qt/                         # ✅ Qt 小项目（计算器、登录界面…）
├── projects/                   # ✅ 综合项目（视觉工具、工业检测系统）
│
├── docs/                       # GitHub Pages 站点（发布目录：/docs）
│   ├── index.html              # ✅ 个人主页（HTML + Canvas 火山实时渲染）
│   ├── 404.html                # ✅ 自定义 404 页面
│   ├── og.png                  # ✅ 社交分享缩略图（1200×630）
│   ├── favicon.svg             # ✅ 网站标签页图标
│   ├── snake-dark.svg          # ⚙️ 贡献蛇（深色）
│   ├── snake-light.svg         # ⚙️ 贡献蛇（浅色）
│   ├── .nojekyll               #    告诉 Pages 不要用 Jekyll 处理
│   └── data/
│       ├── progress.json       # ✅ **唯一进度数据源**（技能 / 路线 / 周计划 / 项目功能）
│       ├── progress.js         # ⚙️ 由 progress.json 生成，主页读取
│       └── commits.json        # ⚙️ 提交数据（最近喷发 / 热力图 / 累计提交）
│
├── notes/                      # ✅ 每日记录
│   ├── 2026-11-12.md           #    第一篇笔记（模板示例）
│   ├── PLAN.md                 #    旧 4 周逐日学习计划（保留参考）
│   ├── 项目理解模板.md            #    时序图/对象表/状态表（每搞懂一个模块填一页）
│   └── bagu/                   #    八股卡片（目前空）
│
├── profile/README.md           # ✅ GitHub 个人主页 README 模板（复制到同名仓库）
├── scripts/gen-assets.mjs      #    progress.json → progress.js + dashboard.svg + README 进度行
├── .gitattributes              #    统一换行符（避免 CRLF 警告）
├── .gitignore                  #    忽略构建产物、视频、大文件
└── README.md                   # ✅ 仓库首页
```

## 想改什么，改哪个文件

| 想改的东西 | 改这个文件 | 生效方式 |
| :-- | :-- | :-- |
| **技能进度**（勾选子项，百分比自动算） | `docs/data/progress.json` → `skills` 的 `subs` | Actions 自动生成，约 1 分钟上线 |
| **当前阶段 / 阶段进度** | `docs/data/progress.json` → `stages` | 同上 |
| **7 天计划** | `docs/data/progress.json` → `days` | 同上 |
| **项目功能清单** | `docs/data/progress.json` → `features` | 同上 |
| **主页文案 / 配色 / 动效** | `docs/index.html` | push 后 Pages 重建 |
| **README 文案** | `README.md` | push 后立即显示 |
| **每日笔记** | 新建 `notes/YYYY-MM-DD.md` | push 后立即显示 |
| **写代码** | `cpp/`、`qt/`、`projects/` 对应子目录 | push 后立即显示 |
| **个人主页门面** | `profile/README.md` 复制到同名仓库 `WMWWZS` | 该仓库推送后生效 |
| **分享缩略图** | `docs/og.png`（1200×630） | push 后平台重新抓取 |
| 贡献蛇 / 提交数据 | 不用管 | Actions 每天自动同步 |

## 三个 Actions 在做什么

| 工作流 | 触发 | 产物 |
| :-- | :-- | :-- |
| `snake.yml` | 每天 UTC 2:00 + 手动 | `docs/snake-dark.svg`、`docs/snake-light.svg` |
| `sync-commits.yml` | 每次 push + 每天 UTC 3:00 | `docs/data/commits.json` |
| `gen-assets.yml` | `progress.json` / 模板 / 生成器变动时 | `docs/data/progress.js`、`assets/dashboard.svg`、README 里的「当前进度」行 |

## 数据流（为什么改一处就够）

```text
docs/data/progress.json  （唯一数据源：只勾子项，百分比自动算）
        │
        ├── gen-assets.mjs ──→ docs/data/progress.js   ──→ 个人主页读取
        ├── gen-assets.mjs ──→ assets/dashboard.svg    ──→ README 显示
        └── gen-assets.mjs ──→ README.md「当前进度」行  ──→ README 显示
```

## 小知识

- `.gitkeep`：Git 不能提交空目录，这个小文件只是用来占位，让 `cpp/`、`qt/` 这类空文件夹能出现在仓库里
- `⚙️` 标记的文件都由 Actions 生成并自动提交，**手改会在下次运行时被覆盖**
- 技能百分比：拖动进度条可 1% 步进微调（会写进 `pct`）；不写 `pct` 时按子项自动算；点勾子项 = 按子项精确计算
- 本地改完想立刻预览：在仓库根目录运行 `node scripts/gen-assets.mjs`，再刷新本地页面即可
- 404 页面：`docs/404.html`，访问不存在的路径时 GitHub Pages 会自动使用它
- `.gitattributes`：统一 LF 换行，Windows 下提交不再刷 CRLF 警告
- 页面上可以直接改进度：技能条拖动（1% 步进）、点项目功能项、拖当前阶段进度、点「推进阶段」；改完点「复制 progress.json」粘贴到仓库提交，Actions 才会同步到全站
