# Turing Machine

## Turning the C language into a language for Turing machines.

This project is *not* a Turing machine compiler written in C. There are already plenty of those.

The goal is to transform C itself into a language for Turing machines.

This has plenty of advantages:
- The initialisation of the tape can involve some computation from the input passed at the beginning of the execution
- The C compiler ensures that the execution is as fast as possible (more optimized than a homebrewn interpreter could manage) and there is no interpretation overhead
- The machine is self-contained and does not require another script to be read (apart from the C compiler itself of course)
- Compiling the machine can be done in a single step: there is no interpreter to compile before use
- The way of displaying the state of the tape can be easily customized on a per-machine basis and does not affect other machines, eliminating the need for as many variants of the interpreter as there are machines

The main downsides I can think of are:
- It requires auxiliary code that is necessary for the C compiler but of no interest to the machine itself (most notably `#include <stdio.h>` and `int main () {`)
- The logic is repeated in multiple files, making it harder to apply a change to all machines
- The extensive use of macros may slow down the compiler a bit, as a lot of useless code is generated before the compiler has to optimize it away. (see NOTE #1)
- The macro itself is bound to either be unreadable due to lack of proper indentation and formating, or take up more space than necessary and distract from the transition table where the relevant logic is held. I chose the first, but some may prefer a somewhat more readable macro with 20+ lines.
- There are some risks of name collisions: any character declared as a symbol cannot also be used as a variable. Thus 1-character variable names are discouraged.
- There is a restriction on which symbols can be used: only `0-9a-zA-Z` and the blank symbol `_` can be written to the tape.


One main goal is for the syntax to be readable (yes, this is C code):
```c
[...]
DELTA( 32, 51,_,-,  _,_,>, 33,_,>,  _,_,-,  _,_,-)
DELTA( 33, 21,_,>,  _,_,>,  _,_,>,  _,_,-,  _,_,-)
DELTA( 34, 35,_,<,  _,O,>,  _,I,>,  _,_,>,  _,_,>)
DELTA( 35, 36,_,<,  _,_,-,  _,_,-,  _,_,<,  _,_,<)
DELTA( 36, 43,_,>, 37,O,>, 38,I,>,  _,_,<,  _,_,<)
[...]
```

Again, each call to `DELTA` is *not* a command to add a new state, rather it expands into the state itself.

### NOTE #1 : Program size
Taking `prime_test/prime.c` as an example, let us study the size of the machine.

```bash
$ gcc -o prime prime.c
$ gcc -o prime2 -O2 prime.c
$ gcc -o prime3 -O3 prime.c
$ gcc -E prime.c > expanded
```

After removing the included code from `expanded`:
```
-rw-rw-r--     40513    expanded
-rwxrwxr-x     42552    prime
-rwxrwxr-x     26192    prime2
-rwxrwxr-x     26192    prime3
-rw-rw-r--      5163    prime.c
```

The file size is multiplied by 8 during the macro expansion despite the comments being removed! About half of the resulting code is optimized away when `-O2` at least is used, and there is nothing more that `-O3` can do.


## Setup and execution

```bash
$ git clone https://github.com/Vanille-N/turing_machine
$ cd turing_machine
$ gcc -o tmake tmake.c
```

For help use `$ ./tmake -h`.

`tmake` takes a list of symbols, and outputs a template where only the transition table has to be filled in.

Example: `$ ./tmake 01ABC > test.c`

If you are using MSVC, then one of the default constructs will fail to compile, as some compilers do not support the GNU extension for statement expressions. In that case, use `$ ./tmake 01ABC -t > test.c` (the `-t` option should come last).

A file is generated: `test.c`, in which the transition table is left blank. See `incr/README.md` for a more detailed example, and `EXPLAIN.md` for an analysis of the macro itself.
