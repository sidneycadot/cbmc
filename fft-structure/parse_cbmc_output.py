#! /usr/bin/env python3

import re

filename = "x"

state = {}
with open(filename) as f:
    for line in f:
        line = line.rstrip("\r\n")
        if line == "Violated property:":
            break
        if "=" in line:
            lhs, rhs = line.split("=", 1)
            lhs = lhs.strip()
            rhs = rhs.split(" ")[0]
            if rhs == "{":
                continue # ignore array assignment
            elif rhs == "FALSE":
                rhs = False
            elif rhs == "TRUE":
                rhs = True
            elif rhs.endswith("u"):
                rhs = int(rhs[:-1])
            else:
                print("OOPS:", line)
                raise RuntimeError()

            state[lhs] = rhs
        #else:
        #    print("@@@ <{}>".format(line))

for lhs in sorted(state):
    rhs = state[lhs]
    print(lhs, rhs)
