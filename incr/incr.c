#include <stdio.h>

enum SYMBOLS {_=-1, };

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

#define DELTA(q, n_,c_,m_, n0,c0,m0, n1,c1,m1) \
	Q##q: \
	printf("\t{State %d}\t{read %c}\t",q,tsl[9-*head]);for(int idx=100;idx<140;idx++) printf("%s%c",(head==tape+idx)?"\033[31m":"\033[0m" ,tsl[9-tape[idx]]);printf("\033[0m\n"); \
	switch(*head){ \
		case _:*head=(c_==_?_:c_);head+="\0\2\1"[2 m_ 0]-1;if(n_==_){if(c_==_&&2 m_ 0==2){goto Q_;}else{goto Q##q;}}else{goto Q##n_;} \
		case 0:*head=(c0==_?0:c0);head+="\0\2\1"[2 m0 0]-1;if(n0==_){if(c0==_&&2 m0 0==2){goto Q_;}else{goto Q##q;}}else{goto Q##n0;} \
		case 1:*head=(c1==_?1:c1);head+="\0\2\1"[2 m1 0]-1;if(n1==_){if(c1==_&&2 m1 0==2){goto Q_;}else{goto Q##q;}}else{goto Q##n1;} \
	}


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


/******************/
/**/START_STATE:/**/
/******************/
/**/  goto Q0;  /**/
/******************/


/****************************************/
/**/TRANSITION_TABLE:                 /**/
/****************************************/
/*        q:   _:      0:      1:       */
/**/DELTA(  0,  1,_,<,  _,_,>,  _,_,>)/**/
/**/DELTA(  1, OK,1,-, OK,1,-,  _,0,<)/**/
/****************************************/


/***************************************************************************/
/**/END_STATES:                                                          /**/
/***************************************************************************/
/**/QOK: PRINT; printf("\n\n\t\t\t\033[1;32m Accept \033[0m") ; goto Q_ ;/**/
/**/QKO: PRINT; printf("\n\n\t\t\t\033[1;31m Reject \033[0m") ; goto Q_ ;/**/
/**/Q_: printf("done\n") ; return 0 ;                                    /**/
/***************************************************************************/
}
