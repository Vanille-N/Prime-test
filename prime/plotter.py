#!/usr/bin/python3

import time
import os
import matplotlib.pyplot as plt
from matplotlib import rc
from math import log

rc('text', usetex=True)
rc('font', family='serif')

K = {}
try:
    with open("save.txt", 'r') as k:
        for s in k.readlines():
            try:
                n, t = map(int, s.split(" "))
                K[n] = t
            except AttributeError:
                break
except FileNotFoundError:
    pass

def data(n):
    if n in K:
        return K[n]
    os.system("echo {} > in.txt".format(n))
    os.system("./prime < in.txt | wc -l > out.txt")
    with open("out.txt", 'r') as f:
        T = int(f.readline()) - 3
    K[n] = T
    print(n)
    return T

def gather(lo, hi):
    D = []
    for i in range(lo, hi):
        D.append(data(i))
    return D

def fit(F, Y):
    Z = list(zip(Y, F))
    if len(Z) > 10000:
        j = 1000
    else:
        j = min(100, len(Z)//2)
    Z = Z[-j:]
    return max(y/f for y,f in Z)

def disp(lo, hi, theta=[]):
    X = [*range(lo, hi)]
    Y = gather(lo, hi)
    plt.xlabel(r"$N$", size=16)
    plt.yscale('log')
    plt.ylabel(r"$T$", size=16)
    plt.scatter(X, Y, label=r"$f$", c='red', s=0.1)
    for f,l in theta:
        F = [f(x) for x in X]
        c = fit(F, Y)
        plt.plot(X, [f*c for f in F], label=l)
    plt.legend()
    plt.show()

n2 = (lambda n: n**2, r"$\Theta(n^2)$")
n = (lambda n: n, r"$\Theta(n)$")
nlgn = (lambda n: n*log(n), r"$\Theta(n \lg\ n)$")
nsqn = (lambda n: n**1.5, r"$\Theta(n^{3/2})$")
nlg2n = (lambda n: n*log(n)**2, r"$\Theta(n\ lg^2n)$")
nsqnlgn = (lambda n: n**1.5*log(n), r"$\Theta(n \sqrt{n}\ lg\ n)$")
nsqnlg2n = (lambda n: n**1.5*log(n)**2, r"$\Theta(n \sqrt{n}\ lg^2n)$")

disp(1, 3000, theta=[nlg2n])

with open("save.txt", "w") as k:
    for n in K:
        k.write("{} {}\n".format(n, K[n]))
