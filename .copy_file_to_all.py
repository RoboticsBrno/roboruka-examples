#!/usr/bin/env python3

import os
import shutil
import sys

if __name__ == "__main__":
    source = os.path.normpath(sys.argv[1])
    abs_source = os.path.abspath(source)
    abs_pwd = os.path.abspath(".")

    if os.path.commonprefix([ abs_source, abs_pwd ]) != abs_pwd:
        raise Exception()


    tokens = source.split(os.sep, 1)
    if len(tokens) != 2:
        raise Exception()

    rel_path = tokens[1]

    for example_dir in os.listdir("."):
        if not os.path.isdir(example_dir) or example_dir.startswith(".") or example_dir == tokens[0]:
            continue

        dest_path = os.path.join(example_dir, rel_path)
        dest_dir = os.path.dirname(dest_path)
        if not os.path.isdir(dest_dir):
            os.makedirs(dest_dir, 0o0755)

        print(source, "->", dest_path)
        
        if os.path.isdir(source):
            shutil.copytree(source, dest_path, symlinks=True, dirs_exist_ok=True)
        else:
            shutil.copy2(source, dest_path, follow_symlinks=False)
