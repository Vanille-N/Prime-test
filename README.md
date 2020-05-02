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

All successive transitions are printed to `stdout`, and can thus easily be counted by piping the executable to `wc -l`.

Although it is quite difficult to calculate exact complexity of the algorithm used, two things can be said:

- Best-case is almost `O(1)` as testing divisibility by 2 is easy (though it requires moving to the end of the number, thus incurring a `Ω(lg n)`)
- Worst-case seems reasonably close to `Θ(n lg²n)` according to experimental data

Fig1 plots the number of transitions T as a function of the number to be tested N. It is the direct output of executing `plotter.py`.

## How to install and run
```bash
$ git clone https://github.com/Vanille-N/Prime-test
$ cd Prime-test
$ gcc -o prime prime.c
```

For a simple run
```bash
$ ./prime
```
then enter the input as prompted.

For the graph
```bash
$ chmod u+x plotter.py
$ ./plotter
```
or
```bash
$ python3 plotter.py
```

(Requires `matplotlib`)

You may safely delete `save.txt`, as it is only used for caching and will be restored by the script. Be aware though that the cached results are worth several hour of computation.

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
Any transition that leads to `_` (see precision later) is either a final transition (notice that the two states `50 (Accept)` and `51 (Reject)` have a transition to `_`) or an error after the program was put in a configuration that is impossible under correct execution (e.g. `DELTA(  0,  1,_,<,  _,_,>,  _,_,>,  _,_,-,  _,_,-)`, at the very beginning the tape is composed only of `o`s and `i` (and `_`s), seeing a `O` results in aborting the program).

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
            - `_ => 0 => '_'`
            - `o => 1 => '0'`
            - `i => 2 => '1'`
            - `O => 3 => 'O'`
            - `I => 4 => 'I'`
        - the position of the head is printed in red using ANSI codes
            - `(h == tape + j) ? "\033[31m" : "\033[0m"`
            - `^~under~head~?.....^~red~~~.....^~white`
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

#### NOTE #1 : The writer
```c
*h = (c_ == _ ? _ : c_) ;
```
The `c_` (more generally `cx`) parameter indicates the symbol to write when `_` (`x`) is read, but there is one exception: it is impossible to write `_`! The symbol `_` only represents a tape cell that has never been overwritten.

When `i`, `o`, `I` or `O` are provided, they are written, but `_` means that the tape should not be overwritten.

This behavior allows us to focus on the transitions that are interesting: the parts of the transitions where nothing happens are marked as `_` or `-` and do not stand out, which makes reading the machine a bit easier.

It is important to note that all `_` here have different meanings : the '`_`' in `c_` means "the symbol that was read", the first '`_`' means "do not overwrite", which is translated into the second '`_`': "rewrite the same symbol".

Indeed, the next case `*h = (co == _ ? o : co)` is proof that the `_` changed to `_` and the `_` changed to `o` have different meanings.


#### NOTE #2 : Move
```c
h += "\000\002\001"[2 m_ 0] - 1 ;
```

This is probably the trick I'm the most proud of. At first sight it seems unreadable, but thanks to this the syntax for the transition is a lot more readable.

Earlier versions of the machine used the following:
```c
#define L -1      // Left
#define R 1       // Right
```
and the line in question was replaced with
```c
h += (m_ == _ ? 0 : m_) ;
```
which was just a more secure way of writing
```c
h += m_ ;
```
because it did not assume that `_` was necessarily `0`.

Thus transitions looked like this :
```c
DELTA(  0,  1,_,L,  _,_,R,  _,_,R,  _,_,_,  _,_,_)
```

On the other hand, consider the much more elegant :
```c
DELTA(  0,  1,_,<,  _,_,>,  _,_,>,  _,_,-,  _,_,-)
```
Left and right are immediately understood without the need for mentally translating `L` and `R` into full words, and the "do not move" transition now has a distinct appearance from the other `_`. NOTE #3 shows that the movement plays a particular role, and deserves a different symbol.

To understand how it works, let's consider a simpler case and expand the macro using https://godbolt.org/ with compiler option `-E` :

```c
#define move(op) "\000\002\001"[2 op 0]-1

move(>)
move(<)
move(-)
```
translates to
```c
"\000\002\001"[2 > 0]-1
"\000\002\001"[2 < 0]-1
"\000\002\001"[2 - 0]-1
```

We once again use string indexing to map `int -> int`.

```
move(>)    -->    "\000\002\001"[2 > 0]-1    -->    "\000\002\001"[1]-1    -->    '\002'-1    -->    1
move(<)    -->    "\000\002\001"[2 < 0]-1    -->    "\000\002\001"[0]-1    -->    '\000'-1    -->   -1
move(-)    -->    "\000\002\001"[2 - 0]-1    -->    "\000\002\001"[2]-1    -->    '\001'-1    -->    0
```
And we have a macro that maps `<`, `-`, `>` to `-1`, `0`, `1`!

#### NOTE #3 : State transition
```c
if (n_ == _) {
    if (c_ == _ && 2 m_ 0 == 2) {
        goto Q_ ;
    } else {
        goto Q##q ;
    }
} else {
    goto Q##n_ ;
}
```
Due to some design decisions, there are some complex combinatorics involved. When a next state is provided the transition is done to that state (`goto Q##n_`), but when no state is given there are two possibilities : it can mean either not to move, or to go to the garbage state.

The behavior is determined by the contents of `mx`. When no movement is done, the transition is considered blank: `_,_,-` expands to "abort execution without writing anything", while `_,_,<` means "do not overwrite, move left, stay in the same state" and `_,o,>` means "write `o`, move right, stay in the same state.

As it is essentially useless to write without moving, taking into account the value of `cx` in the interpretation of `nx` was deemed unnecessary.

The `2 m_ 0 == 2` test a subset of the trick explained in NOTE #2: it tests whether `m_` is `-`
