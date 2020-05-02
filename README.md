# Prime-test

## A Turing machine to test primality

Thanks to the power of the C preprocessor and with an unrestricted use of `gotos`, it is possible to create a beautiful macro:
```c
#define DELTA(q, n_,c_,m_, no,co,mo, ni,ci,mi, nO,cO,mO, nI,cI,mI) \
    Q##q: \
    printf("\t{State %d}\t{read %c}\t",q,"_01OI"[*h]);for(int j=100;j<140;j++)printf("%s%c",(h==tape+j)?"\033[31m":"\033[0m" ,"_01OI"[tape[j]]);printf("\033[0m\n"); \
    switch(*h){ \
        case _:*h=(c_==_?_:c_);h+="\000\002\001"[2 m_ 0]-1; if(n_==_){if(c_==_&&2 m_ 0==2){goto Q_;}else{goto Q##q;}}else{goto Q##n_;} \
        case o:*h=(co==_?o:co);h+="\000\002\001"[2 mo 0]-1; if(no==_){if(co==_&&2 mo 0==2){goto Q_;}else{goto Q##q;}}else{goto Q##no;} \
        case i:*h=(ci==_?i:ci);h+="\000\002\001"[2 mi 0]-1; if(ni==_){if(ci==_&&2 mi 0==2){goto Q_;}else{goto Q##q;}}else{goto Q##ni;} \
        case O:*h=(cO==_?O:cO);h+="\000\002\001"[2 mO 0]-1; if(nO==_){if(cO==_&&2 mO 0==2){goto Q_;}else{goto Q##q;}}else{goto Q##nO;} \
        case I:*h=(cI==_?I:cI);h+="\000\002\001"[2 mI 0]-1; if(nI==_){if(cI==_&&2 mI 0==2){goto Q_;}else{goto Q##q;}}else{goto Q##nI;} \
    }
```
that allows us to use C as if it were a Turing machine, with a friendly syntax for the transition function:
```c
[...]
DELTA( 32, 51,_,-,  _,_,>, 33,_,>,  _,_,-,  _,_,-)
DELTA( 33, 21,_,>,  _,_,>,  _,_,>,  _,_,-,  _,_,-)
DELTA( 34, 35,_,<,  _,O,>,  _,I,>,  _,_,>,  _,_,>)
DELTA( 35, 36,_,<,  _,_,-,  _,_,-,  _,_,<,  _,_,<)
DELTA( 36, 43,_,>, 37,O,>, 38,I,>,  _,_,<,  _,_,<)
[...]
```

This short program implements such a Turing Machine that uses 1 tape, 52 states, and 5 symbols to determine whether a positive integer given as input is a prime number.

All successive transitions are shown.

Although it is quite difficult to calculate exact complexity of the algorithm used, two things can be said:

- Best-case is almost `O(1)` as testing divisibility by 2 is easy
- Worst-case seems reasonably close to `Θ(n lg²n)` according to experimental data

Fig1 plots the number of transitions T as a function of the number to be tested N. It is the direct output of executing `plotter.py`.

## How does it even work ?

I will not go into the details of the algorithm itself, because
- that's not the fun part
- I can't find my notes on what each transition does

I will however explain the auxiliary code (mostly comprised of the macro) in detail.

```c
for(int c=1;n;c<<=1){*(--h)=(n&c)?i:o;n&=~c;}
```
expands to
```c
for (int c = 1; n; c <<= 1) {      // for each bit
    *(--h) = (n & c) ? i : o ;     // write o (0) or i (1) to the tape
    n &= ~c ;                      // clear the bit in the source integer
}
```
and is just a compact way to convert to base 2.


On to the main stuff !
```c
#define DELTA(q, n_,c_,m_, no,co,mo, ni,ci,mi, nO,cO,mO, nI,cI,mI) \
    Q##q: \
    printf("\t{State %d}\t{read %c}\t",q,"_01OI"[*h]);for(int j=100;j<140;j++)printf("%s%c",(h==tape+j)?"\033[31m":"\033[0m" ,"_01OI"[tape[j]]);printf("\033[0m\n"); \
    switch(*h){ \
        case _:*h=(c_==_?_:c_);h+="\000\002\001"[2 m_ 0]-1; if(n_==_){if(c_==_&&2 m_ 0==2){goto Q_;}else{goto Q##q;}}else{goto Q##n_;} \
[...]
```

For each tape symbol (`_`, `o`, `O`, `i`, `I`) there are three parameters :
- `nx` is the identifier of the next state
- `cx` is the symbol to write
- `mx` is the tape movement

One additional parameter `q` is provided : it indicates the identifier of the current state. Any other state that has this state's `q` as one of its `nx`s may result in a transition to this state.

One special state `_` is the garbage state.
Any transition that leads to `_` (see precision later) is either a final transition (notice that the two states `50 (Accept)` and `51 (Reject)` have a transition to `_`) or an error after the program was put in a configuration that is impossible under correct execution (e.g. `DELTA(  0,  1,_,<,  _,_,>,  _,_,>,  _,_,-,  _,_,-)`, the tape is composed only of `o`s and `i`, seeing a `O` results in aborting the program).

```c
Q##q:

printf("\t{State %d}\t{read %c}\t", q, "_01OI"[*h]) ;
for (int j = 100; j < 140; j++)
    printf("%s%c", (h == tape + j) ? "\033[31m"
                                   : "\033[0m"
                 , "_01OI"[tape[j]]
          ) ;
    printf("\033[0m\n") ;

switch (*h) {
    case _:
        *h = (c_ == _ ? _ : c_) ;
        h += "\000\002\001"[2 m_ 0] - 1 ;
        if (n_ == _) {
            if (c_ == _ && 2 m_ 0 == 2) {
                goto Q_ ;
            } else {
                goto Q##q ;
            }
        } else {
            goto Q##n_ ;
        }
[...]
```

There is a lot to unpack :
- `q` is made into the label `Q##q:` for external `goto`s
- the macro first prints some information
    - the current state identifier
    - the symbol under the head
    - a state of the tape
        - `o`, `i`, `O`, `I`, `_` are printed as `0`, `1`, `O`, `I`, `_`
        - `"_01OI"[*h]` serves as a translator :
            - _ => 0 => '_'
            - o => 1 => '0'
            - i => 2 => '1'
            - O => 3 => 'O'
            - I => 4 => 'I'
        - the position of the head is printed in red using ANSI codes
            - (h == tape + j) ? "\033[31m" : "\033[0m"
            - ^ under head ?    ^ red        ^ white
- then the main logic for the transition comes
```c
switch (*h) {                              // match the current symbol
    case _:                                // only one case is provided, the others behave the same way
        *h = (c_ == _ ? _ : c_) ;          // write the tape, see NOTE #1
        h += "\000\002\001"[2 m_ 0] - 1 ;  // move the head, see NOTE #2
        if (n_ == _) {                     // change to next state, see NOTE #3
            if (c_ == _ && 2 m_ 0 == 2) {
                goto Q_ ;
            } else {
                goto Q##q ;
            }
        } else {
            goto Q##n_ ;
        }
[...]
```
