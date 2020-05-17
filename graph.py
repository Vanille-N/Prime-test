#!/usr/bin/python3

import re
from graphviz import Digraph

graph = [] # list of graph edges
symb = [] # list of symbols

is_ok_used = False
is_ko_used = False

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
        for s,b in zip(symb, blocks[1:]):
            t = re.search(r" *(OK|KO|_|[0-9]+),(.),(.)", b)
            nq, ns, mv = t.groups()
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

print(graph, symb)

def make_label(s, ns, mv):
    if ns == '_':
        if mv == '-':
            return '{}:'.format(s)
        else:
            return '{}:({})'.format(s, mv)
    else:
        if mv == '-':
            return '{}:{}'.format(s, ns)
        else:
            return '{}:{}({})'.format(s, ns, mv)


f = Digraph('Turing machine', filename='fsm.gv')
f.attr(rankdir='LR', size='8,5')

f.attr('node', shape='doublecircle')
f.node('q0')
if is_ok_used: f.node('qOK')
if is_ko_used: f.node('qKO')

f.attr('node', shape='circle')
for (s, q, nq, ns, mv) in graph:
    f.edge('q{}'.format(q), 'q{}'.format(nq), label=make_label(s, ns, mv))

f.view()
