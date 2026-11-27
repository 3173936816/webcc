#!/usr/bin/env python3

import argparse
import os
import shutil
import sys

TEMPLATE_DIR = os.path.normpath(
    os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "template")
)

# 所有 dev-templates 项目共享的依赖缓存，可用 WEBCC_DEPS_CACHE 覆盖
CACHE_ROOT = os.environ.get("WEBCC_DEPS_CACHE") or os.path.expanduser("~/.cache/webcc-deps")


def copy_template(target):
    if os.path.exists(target):
        raise ValueError("target already exists: %s" % target)

    count = 0
    for dirpath, dirnames, filenames in os.walk(TEMPLATE_DIR):
        dirnames.sort()
        rel_dir = os.path.relpath(dirpath, TEMPLATE_DIR)
        out_dir = target if rel_dir == "." else os.path.join(target, rel_dir)
        os.makedirs(out_dir, exist_ok=True)

        # .gitkeep 只是为了让 template 的空目录能进 git，不直接复制
        real_files = [name for name in sorted(filenames) if name != ".gitkeep"]

        if real_files:
            for filename in real_files:
                src = os.path.join(dirpath, filename)
                dst = os.path.join(out_dir, filename)
                shutil.copy2(src, dst)
                count += 1
        elif rel_dir != ".":
            # 模板里没有文件的目录，生成 .gitkeep 占位，保证能提交到 git
            open(os.path.join(out_dir, ".gitkeep"), "w").close()
            count += 1

    return count


def link_deps(target):
    """把 deps 下的 sources/builds/packages 链接到共享缓存，避免重复下载。"""
    deps_dir = os.path.join(target, "deps")
    if not os.path.isdir(deps_dir):
        return

    for name in ("sources", "builds", "packages"):
        cache_dir = os.path.join(CACHE_ROOT, name)
        os.makedirs(cache_dir, exist_ok=True)

        link_path = os.path.join(deps_dir, name)
        if os.path.lexists(link_path):
            continue

        try:
            os.symlink(cache_dir, link_path)
        except OSError as e:
            print("Warning: failed to link %s: %s" % (link_path, e), file=sys.stderr)


def main():
    parser = argparse.ArgumentParser(description="Copy template/ to a target directory")
    parser.add_argument("target", help="target directory to create")
    args = parser.parse_args()

    if not os.path.isdir(TEMPLATE_DIR):
        print("Error: template directory not found: %s" % TEMPLATE_DIR, file=sys.stderr)
        return 1

    try:
        count = copy_template(args.target)
        link_deps(args.target)
    except ValueError as e:
        print("Error: %s" % e, file=sys.stderr)
        return 1

    print("Copied %d file(s) into %s" % (count, args.target))
    return 0


if __name__ == "__main__":
    sys.exit(main())
