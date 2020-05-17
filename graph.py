#!/usr/bin/python3

import re

graph = [] # list of graph edges
symb = [] # list of symbols

while True:
    try:
        line = input()
    except EOFError:
        break

    if line[0:2] != "/*":
        # useless line
        continue
    if "DELTA" in line:
        # transition
        blocks = line.split(', ')
        curr_state = int(re.search(r".*?([0-9]+)$", blocks[0]).group(1))
        for b in blocks[1:]:
            t = re.search(r" *(OK|KO|_|[0-9]+),(.),(.)", b)
            nq, ns, mv = t.groups()
            if nq == '_' and ns == '_' and mv == '-':
                continue
            if nq == '_':
                nq = str(curr_state)
            print(curr_state, nq, ns, mv)
    elif "q:" in line:
        # table header
        blocks = line.split(':')
        for b in blocks[1:-1]:
            t = re.search(r" *(.)$", b)
            symb.append(t.group(1))
    else:
        # useless line
        continue
