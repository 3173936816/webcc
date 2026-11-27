#!/usr/bin/env python3
"""根据 template/ 生成项目骨架。

目录结构完全由 template/ 定义：
  - template/ 中的每个目录都会在目标中创建；
  - 其中每个文件按 {{placeholder}} 渲染后写入目标；
  - 以 .gitkeep 结尾的文件只用于让空目录进 git，不会被输出。

占位符可用的值：{{name}}。
"""

import argparse
import os
import re
import sys

TEMPLATE_DIR = os.path.normpath(
    os.path.join(os.path.dirname(os.path.abspath(__file__)), "template")
)

PLACEHOLDER_RE = re.compile(r"\{\{\s*([A-Za-z_][A-Za-z0-9_]*)\s*\}\}")


class TemplateError(Exception):
    pass


def render(text, values, rel_path):
    missing = set()

    def replace(match):
        key = match.group(1)
        if key not in values:
            missing.add(key)
            return match.group(0)
        return str(values[key])

    rendered = PLACEHOLDER_RE.sub(replace, text)
    if missing:
        raise TemplateError(
            "%s: unknown placeholder(s): %s"
            % (rel_path, ", ".join(sorted(missing)))
        )
    return rendered


def generate(target, values):
    if os.path.exists(target):
        raise TemplateError("target already exists: %s" % target)

    count = 0
    for dirpath, dirnames, filenames in os.walk(TEMPLATE_DIR):
        dirnames.sort()
        rel_dir = os.path.relpath(dirpath, TEMPLATE_DIR)
        out_dir = target if rel_dir == "." else os.path.join(target, rel_dir)
        os.makedirs(out_dir, exist_ok=True)

        for filename in sorted(filenames):
            if filename == ".gitkeep":
                continue

            src = os.path.join(dirpath, filename)
            rel_path = os.path.relpath(src, TEMPLATE_DIR)
            with open(src, "r", encoding="utf-8") as f:
                text = f.read()

            out_path = os.path.join(out_dir, filename)
            with open(out_path, "w", encoding="utf-8", newline="") as f:
                f.write(render(text, values, rel_path))
            count += 1

    return count


def main():
    parser = argparse.ArgumentParser(description="Generate a project from template/")
    parser.add_argument("target", help="target directory to create")
    parser.add_argument("--name", default=None, help="project name (default: target basename)")
    args = parser.parse_args()

    if not os.path.isdir(TEMPLATE_DIR):
        print("Error: template directory not found: %s" % TEMPLATE_DIR, file=sys.stderr)
        return 1

    name = args.name if args.name is not None else os.path.basename(os.path.abspath(args.target))
    values = {"name": name}

    try:
        count = generate(args.target, values)
    except TemplateError as e:
        print("Error: %s" % e, file=sys.stderr)
        return 1

    print("Generated %d file(s) into %s (name=%s)" % (count, args.target, name))
    return 0


if __name__ == "__main__":
    sys.exit(main())
