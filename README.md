# Prime-test
A Turing machine to test primality

Thanks to the power of the C preprocessor and with an unrestricted use of `gotos`, it is possible to create a beautiful macro:
```c
#define DELTA(q, n_, c_, m_, no, co, mo, ni, ci, mi, nO, cO, mO, nI, cI, mI) \
    Q##q: \
        printf("State %d read %c ", q, translate[*h]) ; \
    for (int j = 100; j < 140; j++) printf("%c", translate[tape[j]]) ; \
    putchar('\n') ; \
	switch (*h) { \
        case _: *h=(c_==_?_:c_) ; h+=(m_==_?0:m_) ; if(n_==_){if(c_==_&&m_==_){goto Q_;}else{goto Q##q;}}else{goto Q##n_;} \
        case o: *h=(co==_?o:co) ; h+=(mo==_?0:mo) ; if(no==_){if(co==_&&mo==_){goto Q_;}else{goto Q##q;}}else{goto Q##no;} \
        case i: *h=(ci==_?i:ci) ; h+=(mi==_?0:mi) ; if(ni==_){if(ci==_&&mi==_){goto Q_;}else{goto Q##q;}}else{goto Q##ni;} \
        case O: *h=(cO==_?O:cO) ; h+=(mO==_?0:mO) ; if(nO==_){if(cO==_&&mO==_){goto Q_;}else{goto Q##q;}}else{goto Q##nO;} \
        case I: *h=(cI==_?I:cI) ; h+=(mI==_?0:mI) ; if(nI==_){if(cI==_&&mI==_){goto Q_;}else{goto Q##q;}}else{goto Q##nI;} \
    }
```
that allows us to use C as if it were a Turing machine, with a friendly syntax for the transition function:
```c
[...]
DELTA( 32, 51,_,_,  _,_,R, 33,_,R,  _,_,_,  _,_,_)
DELTA( 33, 21,_,R,  _,_,R,  _,_,R,  _,_,_,  _,_,_)
DELTA( 34, 35,_,L,  _,O,R,  _,I,R,  _,_,R,  _,_,R)
DELTA( 35, 36,_,L,  _,_,_,  _,_,_,  _,_,L,  _,_,L)
DELTA( 36, 43,_,R, 37,O,R, 38,I,R,  _,_,L,  _,_,L)
[...]
```

This short program implements such a Turing Machine that uses 1 tape, 52 states, and 5 symbols to determine whether a positive integer given as input is a prime number.

All successive transitions are shown.

Although it is quite difficult to calculate exact complexity of the algorithm used, two things can be said:

- Best-case is `O(1)` as testing divisibility by 2 is easy
- Worst-case seems reasonably close to `Θ(n lg²n)` according to experimental data

Fig1 plots the number of transitions T as a function of the number to be tested N. It is the direct output of executing `plotter.py`.
