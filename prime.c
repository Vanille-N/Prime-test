#include <stdio.h>

#define _ 0
#define o 1
#define i 2
#define O 3
#define I 4
#define R 1
#define L -1

static int tape [246] ;
int * h = tape + 128 ;

static char translate [5] = {'_', '0', '1', 'O', 'I'} ;

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

int main () {
    int n ;
    scanf("%d", &n);
    for(int c=1;n;c<<=1){*(--h)=(n&c)?i:o;n&=~c;}
    DELTA(  0,  1,_,L,  _,_,R,  _,_,R,  _,_,_,  _,_,_)
    DELTA(  1,  _,_,_, 48,_,L,  2,_,L,  _,_,_,  _,_,_)
    DELTA(  2, 51,_,_,  3,_,L,  4,_,L,  _,_,_,  _,_,_)
    DELTA(  3, 51,_,_,  _,_,L,  5,_,R,  _,_,_,  _,_,_)
    DELTA(  4, 50,_,_,  _,_,L,  5,_,R,  _,_,_,  _,_,_)
    DELTA(  5,  6,_,L,  _,_,R,  _,_,R,  _,_,_,  _,_,_)
    DELTA(  6, 12,_,R,  7,O,L,  8,I,L,  _,_,_,  _,_,_)
    DELTA(  7,  9,_,L,  _,_,L,  _,_,L,  _,_,_,  _,_,_)
    DELTA(  8, 10,_,L,  _,_,L,  _,_,L,  _,_,_,  _,_,_)
    DELTA(  9, 11,o,R,  _,_,L,  _,_,L,  _,_,_,  _,_,_)
    DELTA( 10, 11,i,R,  _,_,L,  _,_,L,  _,_,_,  _,_,_)
    DELTA( 11,  _,_,R,  _,_,R,  _,_,R,  6,_,L,  6,_,L)
    DELTA( 12, 18,_,R,  _,_,_,  _,_,_, 13,o,R, 13,i,R)
    DELTA( 13, 18,_,R,  _,_,_,  _,_,_, 14,o,R, 14,i,R)
    DELTA( 14, 15,_,R,  _,_,_,  _,_,_,  _,_,R,  _,_,R)
    DELTA( 15, 16,o,L,  _,_,R,  _,_,_,  _,_,_,  _,_,_)
    DELTA( 16, 17,_,L,  _,_,L,  _,_,_,  _,_,_,  _,_,_)
    DELTA( 17,  _,_,_, 12,_,R, 12,_,R,  _,_,L,  _,_,L)
    DELTA( 18, 19,i,L,  _,_,R,  _,_,_,  _,_,_,  _,_,_)
    DELTA( 19,  _,_,_, 20,i,L,  _,_,_,  _,_,_,  _,_,_)
    DELTA( 20, 21,_,R,  _,_,L,  _,_,_,  _,_,_,  _,_,_)
    DELTA( 21, 22,_,L,  _,_,R,  _,_,R, 22,_,L, 22,_,L)
    DELTA( 22, 29,_,R, 24,O,L, 23,I,L,  _,_,_,  _,_,_)
    DELTA( 23, 25,_,L,  _,_,L,  _,_,L,  _,_,_,  _,_,_)
    DELTA( 24, 26,_,L,  _,_,L,  _,_,L,  _,_,_,  _,_,_)
    DELTA( 25,  _,_,_, 27,I,L, 28,O,L,  _,_,L,  _,_,L)
    DELTA( 26,  _,_,_, 28,O,R, 28,I,R,  _,_,L,  _,_,L)
    DELTA( 27, 34,_,R,  _,i,L, 28,o,R,  _,_,_,  _,_,_)
    DELTA( 28, 21,_,R,  _,_,R,  _,_,R,  _,_,R,  _,_,R)
    DELTA( 29, 30,_,L,  _,_,_,  _,_,_,  _,o,R,  _,i,R)
    DELTA( 30, 31,_,L,  _,_,L,  _,_,L,  _,_,_,  _,_,_)
    DELTA( 31, 32,_,R,  _,_,L,  _,_,L,  _,o,L,  _,i,L)
    DELTA( 32, 51,_,_,  _,_,R, 33,_,R,  _,_,_,  _,_,_)
    DELTA( 33, 21,_,R,  _,_,R,  _,_,R,  _,_,_,  _,_,_)
    DELTA( 34, 35,_,L,  _,O,R,  _,I,R,  _,_,R,  _,_,R)
    DELTA( 35, 36,_,L,  _,_,_,  _,_,_,  _,_,L,  _,_,L)
    DELTA( 36, 43,_,R, 37,O,R, 38,I,R,  _,_,L,  _,_,L)
    DELTA( 37, 39,_,R,  _,_,_,  _,_,_,  _,_,R,  _,_,R)
    DELTA( 38, 40,_,R,  _,_,_,  _,_,_,  _,_,R,  _,_,R)
    DELTA( 39, 41,_,L, 41,_,L, 41,_,L,  _,_,R,  _,_,R)
    DELTA( 40, 42,_,L, 42,_,L, 42,_,L,  _,_,R,  _,_,R)
    DELTA( 41,  _,_,_,  _,_,_,  _,_,_, 35,o,L, 35,o,L)
    DELTA( 42,  _,_,_,  _,_,_,  _,_,_, 35,i,L, 35,i,L)
    DELTA( 43, 44,_,R,  _,_,_,  _,_,_,  _,o,R,  _,i,R)
    DELTA( 44, 45,_,R,  _,_,R,  _,_,R,  _,_,_,  _,_,_)
    DELTA( 45, 46,_,L,  _,_,R,  _,_,R,  _,o,R,  _,i,R)
    DELTA( 46,  _,_,_, 47,_,L, 47,_,L,  _,_,_,  _,_,_)
    DELTA( 47, 50,_,_, 21,i,R,  _,o,L,  _,_,_,  _,_,_)
    DELTA( 48, 51,_,_, 51,_,_, 49,_,L,  _,_,_,  _,_,_)
    DELTA( 49, 50,_,_,  _,_,L, 51,_,_,  _,_,_,  _,_,_)
    Q50: printf("\033[1;32m Accept \033[0m") ; goto Q_ ;
    Q51: printf("\033[1;31m Reject \033[0m") ; goto Q_ ;
    Q_: printf("done\n") ; return 0 ;
}
