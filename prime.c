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

#define DELTA(q, n_,c_,m_, no,co,mo, ni,ci,mi, nO,cO,mO, nI,cI,mI) \
    Q##q: \
    printf("\t{State %d}\t{read %c}\t",q,"_01OI"[*h]);for(int j=100;j<140;j++)printf("%s%c",(h==tape+j)?"\033[31m":"\033[0m" ,"_01OI"[tape[j]]);printf("\033[0m\n"); \
    switch(*h){ \
        case _:*h=(c_==_?_:c_);h+="\000\002\001"[2 m_ 0]-1; (n_==_)?({(c_==_&&2 m_ 0==2)?({goto Q_;}):({goto Q##q;});}):({goto Q##n_;}); \
        case o:*h=(co==_?o:co);h+="\000\002\001"[2 mo 0]-1; (no==_)?({(co==_&&2 mo 0==2)?({goto Q_;}):({goto Q##q;});}):({goto Q##no;}); \
        case i:*h=(ci==_?i:ci);h+="\000\002\001"[2 mi 0]-1; (ni==_)?({(ci==_&&2 mi 0==2)?({goto Q_;}):({goto Q##q;});}):({goto Q##ni;}); \
        case O:*h=(cO==_?O:cO);h+="\000\002\001"[2 mO 0]-1; (nO==_)?({(cO==_&&2 mO 0==2)?({goto Q_;}):({goto Q##q;});}):({goto Q##nO;}); \
        case I:*h=(cI==_?I:cI);h+="\000\002\001"[2 mI 0]-1; (nI==_)?({(cI==_&&2 mI 0==2)?({goto Q_;}):({goto Q##q;});}):({goto Q##nI;}); \
    }

int main () {
    int n ;
    printf("  input > ");
    scanf("%d", &n);
    for(int c=1;n;c<<=1){*(--h)=(n&c)?i:o;n&=~c;}
    DELTA(  0,  1,_,<,  _,_,>,  _,_,>,  _,_,-,  _,_,-)
    DELTA(  1,  _,_,-, 48,_,<,  2,_,<,  _,_,-,  _,_,-)
    DELTA(  2, 51,_,-,  3,_,<,  4,_,<,  _,_,-,  _,_,-)
    DELTA(  3, 51,_,-,  _,_,<,  5,_,>,  _,_,-,  _,_,-)
    DELTA(  4, 50,_,-,  _,_,<,  5,_,>,  _,_,-,  _,_,-)
    DELTA(  5,  6,_,<,  _,_,>,  _,_,>,  _,_,-,  _,_,-)
    DELTA(  6, 12,_,>,  7,O,<,  8,I,<,  _,_,-,  _,_,-)
    DELTA(  7,  9,_,<,  _,_,<,  _,_,<,  _,_,-,  _,_,-)
    DELTA(  8, 10,_,<,  _,_,<,  _,_,<,  _,_,-,  _,_,-)
    DELTA(  9, 11,o,>,  _,_,<,  _,_,<,  _,_,-,  _,_,-)
    DELTA( 10, 11,i,>,  _,_,<,  _,_,<,  _,_,-,  _,_,-)
    DELTA( 11,  _,_,>,  _,_,>,  _,_,>,  6,_,<,  6,_,<)
    DELTA( 12, 18,_,>,  _,_,-,  _,_,-, 13,o,>, 13,i,>)
    DELTA( 13, 18,_,>,  _,_,-,  _,_,-, 14,o,>, 14,i,>)
    DELTA( 14, 15,_,>,  _,_,-,  _,_,-,  _,_,>,  _,_,>)
    DELTA( 15, 16,o,<,  _,_,>,  _,_,-,  _,_,-,  _,_,-)
    DELTA( 16, 17,_,<,  _,_,<,  _,_,-,  _,_,-,  _,_,-)
    DELTA( 17,  _,_,-, 12,_,>, 12,_,>,  _,_,<,  _,_,<)
    DELTA( 18, 19,i,<,  _,_,>,  _,_,-,  _,_,-,  _,_,-)
    DELTA( 19,  _,_,-, 20,i,<,  _,_,-,  _,_,-,  _,_,-)
    DELTA( 20, 21,_,>,  _,_,<,  _,_,-,  _,_,-,  _,_,-)
    DELTA( 21, 22,_,<,  _,_,>,  _,_,>, 22,_,<, 22,_,<)
    DELTA( 22, 29,_,>, 24,O,<, 23,I,<,  _,_,-,  _,_,-)
    DELTA( 23, 25,_,<,  _,_,<,  _,_,<,  _,_,-,  _,_,-)
    DELTA( 24, 26,_,<,  _,_,<,  _,_,<,  _,_,-,  _,_,-)
    DELTA( 25,  _,_,-, 27,I,<, 28,O,<,  _,_,<,  _,_,<)
    DELTA( 26,  _,_,-, 28,O,>, 28,I,>,  _,_,<,  _,_,<)
    DELTA( 27, 34,_,>,  _,i,<, 28,o,>,  _,_,-,  _,_,-)
    DELTA( 28, 21,_,>,  _,_,>,  _,_,>,  _,_,>,  _,_,>)
    DELTA( 29, 30,_,<,  _,_,-,  _,_,-,  _,o,>,  _,i,>)
    DELTA( 30, 31,_,<,  _,_,<,  _,_,<,  _,_,-,  _,_,-)
    DELTA( 31, 32,_,>,  _,_,<,  _,_,<,  _,o,<,  _,i,<)
    DELTA( 32, 51,_,-,  _,_,>, 33,_,>,  _,_,-,  _,_,-)
    DELTA( 33, 21,_,>,  _,_,>,  _,_,>,  _,_,-,  _,_,-)
    DELTA( 34, 35,_,<,  _,O,>,  _,I,>,  _,_,>,  _,_,>)
    DELTA( 35, 36,_,<,  _,_,-,  _,_,-,  _,_,<,  _,_,<)
    DELTA( 36, 43,_,>, 37,O,>, 38,I,>,  _,_,<,  _,_,<)
    DELTA( 37, 39,_,>,  _,_,-,  _,_,-,  _,_,>,  _,_,>)
    DELTA( 38, 40,_,>,  _,_,-,  _,_,-,  _,_,>,  _,_,>)
    DELTA( 39, 41,_,<, 41,_,<, 41,_,<,  _,_,>,  _,_,>)
    DELTA( 40, 42,_,<, 42,_,<, 42,_,<,  _,_,>,  _,_,>)
    DELTA( 41,  _,_,-,  _,_,-,  _,_,-, 35,o,<, 35,o,<)
    DELTA( 42,  _,_,-,  _,_,-,  _,_,-, 35,i,<, 35,i,<)
    DELTA( 43, 44,_,>,  _,_,-,  _,_,-,  _,o,>,  _,i,>)
    DELTA( 44, 45,_,>,  _,_,>,  _,_,>,  _,_,-,  _,_,-)
    DELTA( 45, 46,_,<,  _,_,>,  _,_,>,  _,o,>,  _,i,>)
    DELTA( 46,  _,_,-, 47,_,<, 47,_,<,  _,_,-,  _,_,-)
    DELTA( 47, 50,_,-, 21,i,>,  _,o,<,  _,_,-,  _,_,-)
    DELTA( 48, 51,_,-, 51,_,-, 49,_,<,  _,_,-,  _,_,-)
    DELTA( 49, 50,_,-,  _,_,<, 51,_,-,  _,_,-,  _,_,-)
    Q50: printf("\n\n\t\t\t\033[1;32m Accept \033[0m") ; goto Q_ ;
    Q51: printf("\n\n\t\t\t\033[1;31m Reject \033[0m") ; goto Q_ ;
    Q_: printf("done\n") ; return 0 ;
}
