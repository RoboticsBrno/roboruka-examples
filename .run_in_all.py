#!/usr/bin/env python3

import os
import subprocess
import sys

if __name__ == "__main__":
    for example_dir in os.listdir("."):
        if not os.path.isdir(example_dir) or example_dir.startswith("."):
            continue

        subprocess.check_call(sys.argv[1:], shell=True, stdout=sys.stdout, stderr=sys.stderr, stdin=sys.stdin, cwd=example_dir)
