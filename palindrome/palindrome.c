#include <stdio.h>

enum SYMBOLS {_=-1, a=-28, b=-29, x=-51, };

#define x4 _,_,_,_
#define x16 x4,x4,x4,x4
#define x64 x16,x16,x16,x16
#define x256 x64,x64,x64,x64
#define OK 1000
#define KO 1001

int tape [] = { x256 } ;
int * head = tape + 128 ;
char tsl [] = "9876543210_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz" ;

#define PRINT printf("\n\n\t\t");for(int idx=100;idx<140;idx++) printf("%s%c", (head==tape+idx)?"\033[31m":"\033[0m",tsl[9-tape[idx]]);printf("\033[0m\n")

#define DELTA(q, n_,c_,m_, na,ca,ma, nb,cb,mb, nx,cx,mx) \
	Q##q: \
	printf("\t{State %d}\t{read %c}\t",q,tsl[9-*head]);for(int idx=100;idx<140;idx++) printf("%s%c",(head==tape+idx)?"\033[31m":"\033[0m" ,tsl[9-tape[idx]]);printf("\033[0m\n"); \
	switch(*head){ \
		case _:*head=(c_==_?_:c_);head+="\0\2\1"[2 m_ 0]-1;if(n_==_){if(c_==_&&2 m_ 0==2){goto Q_;}else{goto Q##q;}}else{goto Q##n_;} \
		case a:*head=(ca==_?a:ca);head+="\0\2\1"[2 ma 0]-1;if(na==_){if(ca==_&&2 ma 0==2){goto Q_;}else{goto Q##q;}}else{goto Q##na;} \
		case b:*head=(cb==_?b:cb);head+="\0\2\1"[2 mb 0]-1;if(nb==_){if(cb==_&&2 mb 0==2){goto Q_;}else{goto Q##q;}}else{goto Q##nb;} \
		case x:*head=(cx==_?x:cx);head+="\0\2\1"[2 mx 0]-1;if(nx==_){if(cx==_&&2 mx 0==2){goto Q_;}else{goto Q##q;}}else{goto Q##nx;} \
	}

int main () {
printf("  input > ");
char in [100];
scanf("%s", in);
int idx;
for (idx = 0; in[idx] != '\0'; idx++);
for (idx--;idx >= 0; idx--) {
    switch (in[idx]) {
        case 'a': *(head--) = a; break;
        case 'b': *(head--) = b; break;
        default: printf("Invalid input\n"); return 1;
    }
}
head++;


/******************/
/**/START_STATE:/**/
/******************/
/**/  goto Q0;  /**/
/******************/


/************************************************/
/**/TRANSITION_TABLE:                         /**/
/************************************************/
/*        q:   _:      a:      b:      x:       */
/**/DELTA(  0, OK,_,-,  1,x,>,  3,x,>, OK,_,-)/**/
/**/DELTA(  1,  2,_,<,  _,_,>,  _,_,>,  2,_,<)/**/
/**/DELTA(  2, OK,_,-,  5,x,<, KO,_,-, OK,_,-)/**/
/**/DELTA(  3,  4,_,<,  _,_,>,  _,_,>,  4,_,<)/**/
/**/DELTA(  4, OK,_,-, KO,_,-,  5,x,<, OK,_,-)/**/
/**/DELTA(  5,  0,_,>,  _,_,<,  _,_,<,  0,_,>)/**/
/************************************************/


/***************************************************************************/
/**/END_STATES:                                                          /**/
/***************************************************************************/
/**/QOK: PRINT; printf("\n\n\t\t\t\033[1;32m Accept \033[0m") ; goto Q_ ;/**/
/**/QKO: PRINT; printf("\n\n\t\t\t\033[1;31m Reject \033[0m") ; goto Q_ ;/**/
/**/Q_: printf("done\n") ; return 0 ;                                    /**/
/***************************************************************************/
}
