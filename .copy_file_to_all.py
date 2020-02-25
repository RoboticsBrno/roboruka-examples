#!/usr/bin/env python3

import os
import sys

if __name__ == "__main__":
    source = os.path.normpath(sys.argv[1])
    abs_source = os.path.abspath(source)
    abs_pwd = os.path.abspath(".")

    if os.path.commonprefix([ abs_source, abs_pwd ]) != abs_pwd:
        raise Exception()


    tokens = source.split(os.sep, 1)
    print(tokens)
    if len(tokens) != 2:
        raise Exception()

    dest = tokens[1]

    for fn in os.listdir("."):
        if not os.path.isdir(fn) or fn.startswith(".") or fn == tokens[0]:
            continue

        with open(source, "rb") as i:
            with open(os.path.join(fn, dest), "wb") as o:
                o.write(i.read())
