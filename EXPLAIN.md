# How it works

## A detailed explanation of the main macro

```c
#define DELTA(q, n_,c_,m_, nx,cx,mx, ...) \
    Q##q: \
    printf("\t{State %d}\t{read %c}\t",q,tsl[*head]);for(int idx=100;idx<140;idx++)printf("%s%c",(head==tape+idx)?"\033[31m":"\033[0m" ,tsl[tape[idx]]);printf("\033[0m\n"); \
    switch(*head){ \
        case _:*head=(c_==_?_:c_);head+="\0\2\1"[2 m_ 0]-1; if(n_==_){if(c_==_&&2 m_ 0==2){goto Q_;}else{goto Q##q;}}else{goto Q##n_;} \
        case 0:*head=(cx==_?x:cx);head+="\0\2\1"[2 mx 0]-1; if(nx==_){if(cx==_&&2 mx 0==2){goto Q_;}else{goto Q##q;}}else{goto Q##nx;} \
[...]
```
(the `-t` option was used, hence the `if/else` blocks)

For each tape symbol (`_`, `x`, ... ) there are three parameters :
- `nx` is the identifier of the next state
- `cx` is the symbol to write
- `mx` is the tape movement

One additional parameter `q` is provided : it indicates the identifier of the current state. Any other state that has this state's `q` as one of its `nx`s may result in a transition to this state.

One special state `_` is the garbage state.
Any transition that leads to `_` (see precision later) is either a final transition (notice that the two states `50 (Accept)` and `51 (Reject)` have a transition to `_`) or an error after the program was put in a configuration that is impossible under correct execution (e.g. `DELTA(  0,  1,_,<,  _,_,>,  _,_,>,  _,_,-,  _,_,-)`, at the very beginning the tape in `prime_test/prime.c` is composed only of `o`s and `i` (and `_`s), seeing a `O` results in aborting the program).

```c
Q##q:

printf("\t{State %d}\t{read %c}\t", q, "_01OI"[*head]) ;
for (int idx = 100; idx < 140; idx++)
    printf("%s%c", (head == tape + idx) ? "\033[31m"
                                   : "\033[0m"
                 , tsl[tape[idx]]
          ) ;
    printf("\033[0m\n") ;

switch (*head) {
    case _:
        *head = (c_ == _ ? _ : c_) ;
        head += "\000\002\001"[2 m_ 0] - 1 ;
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
        - symbols are printed with string indexing (tsl stands for translator)
        - the position of the head is printed in red using ANSI color codes
            - `(head == tape + idx) ? "\033[31m" : "\033[0m"`
            - `^~under~head~?..........^~red~~~.....^~white`
- then the main logic for the transition comes
```c
switch (*head) {                              // match the current symbol
    case _:                                   // only one case is provided, the others behave the same way
        *head = (c_ == _ ? _ : c_) ;          // write the tape, see NOTE #1
        head += "\000\002\001"[2 m_ 0] - 1 ;  // move the head, see NOTE #2
        if (n_ == _) {                        // change to next state, see NOTE #3
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
*head = (c_ == _ ? _ : c_) ;
```
The `c_` (more generally `cx`) parameter indicates the symbol to write when `_` (`x`) is read, but there is one exception: it is impossible to write `_`! The symbol `_` only represents a tape cell that has never been overwritten.

When normal symbols (`0-9a-zA-Z`) are provided, they are written, but `_` means that the tape should not be overwritten.

This behavior allows us to focus on the transitions that are interesting: the parts of the transitions where nothing happens are marked as `_` or `-` and do not stand out, which makes reading the machine a bit easier.

It is important to note that all `_` here have different meanings : the '`_`' in `c_` means "the symbol that was read", the first '`_`' means "do not overwrite", which is translated into the second '`_`': "rewrite the same symbol".

Indeed, the next case `*head = (cx == _ ? x : cx)` is proof that the `_` changed to `_` and the `_` changed to `x` have different meanings.


#### NOTE #2 : Move
```c
head += "\000\002\001"[2 m_ 0] - 1 ;
```

This is probably the trick I'm the most proud of. At first sight it seems unreadable, but thanks to this the syntax for the transition is a lot more readable.

Earlier versions of the machine used the following:
```c
#define L -1      // Left
#define R 1       // Right
```
and the line in question was replaced with
```c
head += (m_ == _ ? 0 : m_) ;
```
which was just a more secure way of writing
```c
head += m_ ;
```
because it did not assume that `_` was necessarily `0`, which it in fact eventually stopped being when `tmake` was created.

Thus transitions looked like this :
```c
DELTA(  0,  1,_,L,  _,_,R,  _,_,R,  _,_,_,  _,_,_)
```

On the other hand, consider the much more elegant :
```c
DELTA(  0,  1,_,<,  _,_,>,  _,_,>,  _,_,-,  _,_,-)
```
Left and right are immediately understood without the need for mentally translating `L` and `R` into full words, and the "do not move" transition now has a distinct appearance from the other `_`. NOTE #3 shows that the movement plays a particular role, and deserves a different symbol.

To understand how it works, let's consider a simpler case and expand the macro using the compiler option `-E` :

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

The behavior is determined by the contents of `m_` (more generally `mx`) and `c_` (`cx`). When no movement is done and no symbol is written, the transition is considered blank: `_,_,-` expands to "abort execution without writing anything", while `_,_,<` means "do not overwrite, move left, stay in the same state" and `_,x,>` means "write `x`, move right, stay in the same state.

The `2 m_ 0 == 2` test is a subset of the trick explained in NOTE #2: it tests whether `m_` is `-`
