#!/usr/bin/python3

import re
from subprocess import check_call
import sys

def reduce(g):
    table = {}
    for (s, q, nq, ns, mv) in g:
        if (q, nq) in table:
            prev = table[(q, nq)]
            new = []
            found = False
            for (ps, pns, pmv) in prev:
                if (not found) and pns == ns and pmv == mv:
                    new.append(("{},{}".format(s, ps), ns, mv))
                    found = True
                else:
                    new.append((ps, pns, pmv))
            if not found:
                new.append((s, ns, mv))
            table[(q, nq)] = tuple(new)
        else:
            table[(q, nq)] = ((s, ns, mv),)
    red = []
    for (q, nq) in table:
        red.append((q, nq, table[(q, nq)]))
    return red

def translate_symbol(s):
    return s.replace('_', '⎵')

def translate_move(mv):
    return {'<': '🡨', '>': '🡪'}[mv]

def make_label(lst):
    label = ""
    for (s, ns, mv) in lst:
        if label != "":
            label += '\n'
        if ns == '_':
            if mv == '-':
                label += '{}'.format(translate_symbol(s))
            else:
                label += '{}:{}'.format(translate_symbol(s), translate_move(mv))
        else:
            if mv == '-':
                label += '{}:[{}]'.format(translate_symbol(s), ns)
            else:
                label += '{}:[{}]{}'.format(translate_symbol(s), ns, translate_move(mv))
    return label

def main(fname):
    graph = [] # list of graph edges
    symb = [] # list of symbols

    is_ok_used = False
    is_ko_used = False

    f = open(fname, 'r')
    fbase = re.search(r"^(.*)\.[^\.]+", fname).group(1) + "-structure"
    print("reading " + fname)

    for line in f.readlines():
        if line[0:2] != "/*":
            # useless line
            continue
        if "DELTA" in line:
            # transition
            blocks = line.split(', ')
            try:
                curr_state = int(re.search(r".*?([0-9]+)$", blocks[0]).group(1))
            except AttributeError:
                print("    | Invalid file format")
                print("    | Could not extract state id from `{}`".format(line.rstrip()))
                return
            for s,b in zip(symb, blocks[1:]):
                t = re.search(r" *(OK|KO|_|[0-9]+),(.),(.)", b)
                try:
                    nq, ns, mv = t.groups()
                except AttributeError:
                    print("    | Invalid file format")
                    print("    | Could not properly parse `{}` in line `{}`".format(b, line.rstrip()))
                    return
                if nq == '_' and ns == '_' and mv == '-':
                    continue
                if nq == '_':
                    nq = str(curr_state)
                graph.append((s, curr_state, nq, ns, mv))
                if nq == 'OK':
                    is_ok_used = True
                elif nq == 'KO':
                    is_ko_used = True
        elif "q:" in line:
            # table header
            blocks = line.split(':')
            for b in blocks[1:-1]:
                t = re.search(r" *(.)$", b)
                symb.append(t.group(1))
        else:
            # useless line
            continue

    red = reduce(graph)

    with open(".graph.dot", 'w') as f:
        f.write('digraph "Turing machine" {\n')
        f.write('    rankdir=LR size="8,5"\n')
        f.write('    node [shape=doublecircle, fontname="monospace"]\n')
        f.write('    q0\n')
        if is_ok_used: f.write('    qOK\n')
        if is_ko_used: f.write('    qKO\n')

        f.write('    node [shape=circle, fontname="monospace"]\n')

        for (q, nq, lst) in red:
            f.write('    q{} -> q{} [label="{}", fontname="monospace"]\n'.format(q, nq, make_label(lst)))
        f.write('}\n')

    check_call(['dot','-Tpdf','.graph.dot','-o','{}.pdf'.format(fbase)])
    print("    | Created `{}.pdf`".format(fbase))
    check_call(['dot','-Tpng','.graph.dot','-o','{}.png'.format(fbase)])
    print("    | Created `{}.png`".format(fbase))

    check_call(['rm','.graph.dot'])

for arg in sys.argv[1:]:
    main(arg)
    print()
