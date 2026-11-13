#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""给 snk 生成的贡献蛇 SVG 加上火山特效（幂等，可重复运行）。

效果：
  1. 暗色/浅色底板（让 README 里像一块火山面板）
  2. 底部熔岩河 + 呼吸光晕 + 上浮气泡
  3. 蛇身改熔岩渐变 + 橙色发光
  4. 上升火星（随机位置/延迟，纯 CSS 动画）
  5. 格子配色改暖色（更贴火山主题）
  6. 有提交的格子发光
  7. 动画速度 11.5s -> 8.5s

用法：python3 scripts/snake_fx.py（在仓库根目录执行）
"""
import random
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
TARGETS = {
    "snake-dark.svg": dict(
        bg="#0d0705",
        glow0="#f97316", glow1="#7f1d1d", o0="0.34", o1="0.14",
        s0="#fde68a", s1="#fb923c", s2="#ea580c",
        spark="#fdba74", spark2="#f97316",
        snake_glow="#f97316", snake_glow2="rgba(220,38,38,.6)",
        lava="#f97316", hot="#fde68a",
        cells="--ce:#241209;--c1:#5a2410;--c2:#9a3412;--c3:#ea580c;--c4:#fde68a",
    ),
    "snake-light.svg": dict(
        bg="#fffaf5",
        glow0="#fb923c", glow1="#fed7aa", o0="0.24", o1="0.12",
        s0="#9a3412", s1="#ea580c", s2="#fde68a",
        spark="#ea580c", spark2="#f97316",
        snake_glow="#ea580c", snake_glow2="rgba(249,115,22,.5)",
        lava="#ea580c", hot="#f97316",
        cells="--ce:#f3e9e0;--c1:#fed7aa;--c2:#fb923c;--c3:#ea580c;--c4:#9a3412",
    ),
}


def build_defs(c):
    return (
        '<defs>\n'
        '  <radialGradient id="volcano-glow" cx="50%" cy="110%" r="90%">\n'
        f'    <stop offset="0%" stop-color="{c["glow0"]}" stop-opacity="{c["o0"]}"/>\n'
        f'    <stop offset="55%" stop-color="{c["glow1"]}" stop-opacity="{c["o1"]}"/>\n'
        f'    <stop offset="100%" stop-color="{c["glow1"]}" stop-opacity="0"/>\n'
        '  </radialGradient>\n'
        '  <linearGradient id="lava-grad" x1="0%" y1="0%" x2="100%" y2="100%">\n'
        f'    <stop offset="0%" stop-color="{c["s0"]}"/>\n'
        f'    <stop offset="45%" stop-color="{c["s1"]}"/>\n'
        f'    <stop offset="100%" stop-color="{c["s2"]}"/>\n'
        '  </linearGradient>\n'
        '  <linearGradient id="lava-river" x1="0%" y1="0%" x2="100%" y2="0%">\n'
        f'    <stop offset="0%" stop-color="{c["lava"]}" stop-opacity="0"/>\n'
        f'    <stop offset="35%" stop-color="{c["hot"]}" stop-opacity="0.55"/>\n'
        f'    <stop offset="65%" stop-color="{c["lava"]}" stop-opacity="0.45"/>\n'
        f'    <stop offset="100%" stop-color="{c["lava"]}" stop-opacity="0"/>\n'
        '  </linearGradient>\n'
        '</defs>'
    )


def build_css(c):
    return (
        f':root{{{c["cells"]}}}'
        '.bg-glow{animation:bgPulse 7s ease-in-out infinite}'
        '@keyframes bgPulse{0%,100%{opacity:.6}50%{opacity:1}}'
        '.lava-river{animation:riverGlow 5s ease-in-out infinite;'
        f'filter:drop-shadow(0 0 6px {c["lava"]})}}'
        '@keyframes riverGlow{0%,100%{opacity:.45}50%{opacity:.85}}'
        '.bubble{animation:bubbleUp 4.5s ease-in-out infinite}'
        '@keyframes bubbleUp{0%,100%{opacity:.1;transform:translateY(0)}50%{opacity:.75;transform:translateY(-5px)}}'
        '.s{fill:url(#lava-grad);'
        f'filter:drop-shadow(0 0 2px {c["snake_glow"]}) drop-shadow(0 0 9px {c["snake_glow2"]})}}'
        f'.c.c3,.c.c4{{filter:drop-shadow(0 0 3px {c["snake_glow"]})}}'
        '.ember{animation:emberRise 5s linear infinite}'
        '@keyframes emberRise{'
        '0%{opacity:0;transform:translate(0,0)}'
        '8%{opacity:.95}'
        '55%{opacity:.6}'
        '100%{opacity:0;transform:translate(-8px,-64px)}}'
    )


def build_embers(c):
    rnd = random.Random(20261112)  # 固定种子：位置稳定，不会天天变
    parts = ['<g class="embers" fill="none">']
    for _ in range(26):
        x = rnd.uniform(20, 850)
        y = rnd.uniform(95, 148)
        r = rnd.choice([1.2, 1.5, 1.8, 2.2, 2.6])
        dur = rnd.choice([4.2, 5.2, 6.2, 7.2])
        delay = rnd.uniform(0, 7)
        color = c["spark"] if rnd.random() < 0.7 else c["spark2"]
        parts.append(
            f'<circle class="ember" cx="{x:.1f}" cy="{y:.1f}" r="{r}" fill="{color}" '
            f'style="animation-duration:{dur}s;animation-delay:-{delay:.1f}s"/>'
        )
    parts.append('</g>')
    return ''.join(parts)


def build_scene(c):
    """底板 + 光晕 + 熔岩河 + 气泡（放在根标签之后，格子之前）。"""
    return (
        f'<rect x="-16" y="-32" width="880" height="192" fill="{c["bg"]}"/>'
        '<rect class="bg-glow" x="-16" y="-32" width="880" height="192" fill="url(#volcano-glow)"/>'
        '<rect class="lava-river" x="0" y="149" width="848" height="6" rx="3" fill="url(#lava-river)"/>'
        f'<g><circle class="bubble" cx="180" cy="152" r="2.4" fill="{c["hot"]}" style="animation-delay:-.8s"/>'
        f'<circle class="bubble" cx="430" cy="151" r="1.9" fill="{c["lava"]}" style="animation-delay:-2.1s"/>'
        f'<circle class="bubble" cx="690" cy="152.5" r="2.2" fill="{c["hot"]}" style="animation-delay:-3.4s"/></g>'
    )


def apply_fx(path: Path, c: dict) -> str:
    s = path.read_text(encoding="utf-8")
    if 'id="volcano-glow"' in s:
        return "already"

    s = s.replace("11500ms", "8500ms")  # 动画加快

    m = re.match(r"<svg\b[^>]*>", s)
    if not m:
        return "no-svg"
    s = s[: m.end()] + build_defs(c) + build_scene(c) + s[m.end():]

    pos = s.find('<rect class="u ')
    if pos < 0:
        pos = s.rfind("</svg>")
    s = s[:pos] + build_embers(c) + s[pos:]

    s = s.replace("</style>", build_css(c) + "</style>", 1)
    path.write_text(s, encoding="utf-8")
    return "ok"


def main():
    changed = []
    for name, cfg in TARGETS.items():
        p = ROOT / "docs" / name
        if not p.exists():
            print(f"skip {name}: not found")
            continue
        r = apply_fx(p, cfg)
        print(f"{name}: {r}")
        if r == "ok":
            changed.append(name)
    if not changed:
        print("nothing changed")


if __name__ == "__main__":
    main()
