# incr

## How to create a new Turing machine

This machine is so simple that it is barely interesting, apart from demonstrating how to use `tmake`.

Once `tmake` built (see README in project root), `$ ./tmake 01 -t > incr/incr.c` was run to create a new template, then the template was copied to `incr/raw.c` before modifying anything. (`incr/raw.c` does not compile)

There are only two places where we have to modify the template:
- input logic after main
- transition table

In this case,
```c
int main () {
printf("  input > ");
/* Enter here the initialisation logic */
```
was replaced with
```c
int main () {
printf("  input > ");
char in [10];
scanf("%s", in);
int idx;
for (idx = 0; in[idx] != '\0'; idx++);
for (idx--;idx >= 0; idx--) {
    switch (in[idx]) {
        case '0': *(head--) = 0; break;
        case '1': *(head--) = 1; break;
        default: printf("Invalid input\n"); return 1;
    }
}
head++;
```
which simply copies the input after checking its validity (only binary numbers are accepted as input by the machine).

`tmake` generated the following transition table:
```c
/****************************************/
/**/TRANSITION_TABLE:                 /**/
/****************************************/
/*        q:   _:      0:      1:       */
/**/DELTA(  ?,  _,_,-,  _,_,-,  _,_,-)/**/
/**/DELTA(  ?,  _,_,-,  _,_,-,  _,_,-)/**/
/**/DELTA(  ?,  _,_,-,  _,_,-,  _,_,-)/**/
/**/DELTA(  ?,  _,_,-,  _,_,-,  _,_,-)/**/
/**/DELTA(  ?,  _,_,-,  _,_,-,  _,_,-)/**/
/**/DELTA(  ?,  _,_,-,  _,_,-,  _,_,-)/**/
/**/DELTA(  ?,  _,_,-,  _,_,-,  _,_,-)/**/
/**/DELTA(  ?,  _,_,-,  _,_,-,  _,_,-)/**/
/**/DELTA(  ?,  _,_,-,  _,_,-,  _,_,-)/**/
/**/DELTA(  ?,  _,_,-,  _,_,-,  _,_,-)/**/
/****************************************/
```
but there are a few things to be done:
- we don't need that many states
- our states need to be numbered
- there is some logic to enter before our machine can work properly

We thus write the following:
```c
/****************************************/
/**/TRANSITION_TABLE:                 /**/
/****************************************/
/*        q:   _:      0:      1:       */
/**/DELTA(  0,  1,_,<,  _,_,>,  _,_,>)/**/
/**/DELTA(  1, OK,1,-, OK,1,-,  _,0,<)/**/
/****************************************/
```
State 0 is the starting state, and makes the head go to the rightmost non-blank digit.
State 1 then carries the increment until a `0` or a `_` are met.
The transitions are to be understood as the following:
- ` 1,_,<,` means "move left, change to state 1" (note that it does NOT write the symbol `_`)
- `OK,1,-,` means "write 1, do not move, stop the execution"
- ` _,0,<,` means "write 0, move left, remain in the same state"

There are more options that are not seen here:
- a transition to state `KO` indicates a negative result
- a blank transition ` _,_,-,` means that the execution should be aborted without giving an answer

Note that ` _,_,>,` is not a blank transition, as a movement occurs. ` _,_,-,` would result in an infinite loop if it were interpreted as a regular transition, thus it would be useless. This is in part why it was chosen to represent an undefined transition, instead of introducing another syntax.



### Examples:

```
input> 100101

_______________________100101___________
[...]
_______________________100110___________

Accept done
```
``
input> 1111111111

____________________1111111111___________
[...]
___________________10000000000___________

Accept done
```
