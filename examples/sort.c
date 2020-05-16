#include <stdio.h>

enum SYMBOLS {_=-1, O=-16, I=-10, };

#define x4 _,_,_,_
#define x16 x4,x4,x4,x4
#define x64 x16,x16,x16,x16
#define x256 x64,x64,x64,x64
#define OK 1000
#define KO 1001

int tape [] = { x256 } ;
int * head = tape + 10 ;
char tsl [] = "9876543210_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz" ;

#define PRINT printf("\n\n\t");for(int idx=8;idx<169;idx++) printf("%s%c", (head==tape+idx)?"\033[31m":"\033[0m",tsl[9-tape[idx]]);printf("\033[0m\n")

#define DELTA(q, n_,c_,m_, n0,c0,m0, n1,c1,m1, nO,cO,mO, nI,cI,mI) \
	Q##q: \
	printf("%d:%c\t",q,tsl[9-*head]);for(int idx=8;idx<169;idx++) printf("%s%c",(head==tape+idx)?"\033[31m":"\033[0m" ,tsl[9-tape[idx]]);printf("\033[0m\n"); \
	switch(*head){ \
		case _:*head=(c_==_?_:c_);head+="\0\2\1"[2 m_ 0]-1;if(n_==_){if(c_==_&&2 m_ 0==2){goto Q_;}else{goto Q##q;}}else{goto Q##n_;} \
		case 0:*head=(c0==_?0:c0);head+="\0\2\1"[2 m0 0]-1;if(n0==_){if(c0==_&&2 m0 0==2){goto Q_;}else{goto Q##q;}}else{goto Q##n0;} \
		case 1:*head=(c1==_?1:c1);head+="\0\2\1"[2 m1 0]-1;if(n1==_){if(c1==_&&2 m1 0==2){goto Q_;}else{goto Q##q;}}else{goto Q##n1;} \
		case O:*head=(cO==_?O:cO);head+="\0\2\1"[2 mO 0]-1;if(nO==_){if(cO==_&&2 mO 0==2){goto Q_;}else{goto Q##q;}}else{goto Q##nO;} \
		case I:*head=(cI==_?I:cI);head+="\0\2\1"[2 mI 0]-1;if(nI==_){if(cI==_&&2 mI 0==2){goto Q_;}else{goto Q##q;}}else{goto Q##nI;} \
	}

int main () {
printf("  input > ");
for (;;) {
    int num; scanf("%d", &num);
    if (num < 0 || num >= 1<<5) {
        break;
    }
    for (int pow = 4; pow >= 0; pow--) {
        *(head++) = (num & (1<<pow)) ? 1 : 0 ;
    }
    head++;
}
head = tape + 10;


/******************/
/**/START_STATE:/**/
/******************/
/**/  goto Q0;  /**/
/******************/


/********************************************************/
/**/TRANSITION_TABLE:                                 /**/
/********************************************************/
/*        q:   _:      0:      1:      O:      I:       */
/**/DELTA(  0,  1,_,<,  _,_,<,  _,_,<,  _,0,<,  _,1,<)/**/
/**/DELTA(  1,  2,_,>,  4,_,-,  4,_,-,  4,_,-,  4,_,-)/**/
/**/DELTA(  2,  3,_,>,  _,_,-,  _,_,-,  _,_,-,  _,_,-)/**/
/**/DELTA(  3,  4,_,>,  _,_,>,  _,_,>,  _,0,>,  _,1,>)/**/
/**/DELTA(  4, OK,_,-,  5,_,-,  5,_,-,  _,_,-,  _,_,-)/**/
/**/DELTA(  5, 40,_,<,  9,O,<,  6,I,<,  _,_,>,  _,_,>)/**/
/**/DELTA(  6,  7,_,<,  _,_,-,  _,_,-,  _,_,<,  _,_,<)/**/
/**/DELTA(  7,  8,_,>,  _,_,<,  _,_,<,  8,_,>,  8,_,>)/**/
/**/DELTA(  8,  _,_,-, 13,_,<, 12,I,>,  _,_,-,  _,_,-)/**/
/**/DELTA(  9, 10,_,<,  _,_,-,  _,_,-,  _,_,<,  _,_,<)/**/
/**/DELTA( 10, 11,_,>,  _,_,<,  _,_,<, 11,_,>, 11,_,>)/**/
/**/DELTA( 11,  _,_,-, 12,O,>, 17,O,>,  _,_,-,  _,_,-)/**/
/**/DELTA( 12,  5,_,>,  _,_,>,  _,_,>,  _,_,>,  _,_,>)/**/
/**/DELTA( 13, 14,_,>,  _,_,<,  _,_,<,  _,_,<,  _,_,<)/**/
/**/DELTA( 14, 15,_,>,  _,_,>,  _,_,>,  _,0,>,  _,1,>)/**/
/**/DELTA( 15, 16,_,<,  _,_,>,  _,_,>,  _,0,>,  _,1,>)/**/
/**/DELTA( 16, 36,_,>,  _,_,<,  _,_,<,  _,_,-,  _,_,-)/**/
/**/DELTA( 17, 18,_,>,  _,_,>,  _,_,>,  _,_,-,  _,_,-)/**/
/**/DELTA( 18, 19,_,<, 19,_,<, 19,_,<,  _,_,>,  _,_,>)/**/
/**/DELTA( 19,  _,_,-,  _,_,-,  _,_,-, 20,I,>,  _,_,-)/**/
/**/DELTA( 20, 33,_,<, 24,O,<, 21,I,<,  _,_,>,  _,_,>)/**/
/**/DELTA( 21, 22,_,<,  _,_,-,  _,_,-,  _,_,<,  _,_,<)/**/
/**/DELTA( 22,  _,_,-,  _,_,<,  _,_,<, 23,_,>, 23,_,>)/**/
/**/DELTA( 23,  _,_,-, 30,I,>, 27,I,>,  _,_,-,  _,_,-)/**/
/**/DELTA( 24, 25,_,<,  _,_,-,  _,_,-,  _,_,<,  _,_,<)/**/
/**/DELTA( 25,  _,_,-,  _,_,<,  _,_,<, 26,_,>, 26,_,>)/**/
/**/DELTA( 26,  _,_,-, 30,O,>, 27,O,>,  _,_,-,  _,_,-)/**/
/**/DELTA( 27, 28,_,>,  _,_,>,  _,_,>,  _,_,-,  _,_,-)/**/
/**/DELTA( 28, 29,_,<, 29,_,<, 29,_,<,  _,_,>,  _,_,>)/**/
/**/DELTA( 29,  _,_,-,  _,_,-,  _,_,-, 20,I,<, 20,I,<)/**/
/**/DELTA( 30, 31,_,>,  _,_,>,  _,_,>,  _,_,-,  _,_,-)/**/
/**/DELTA( 31, 32,_,<, 32,_,<, 32,_,<,  _,_,>,  _,_,>)/**/
/**/DELTA( 32,  _,_,-,  _,_,-,  _,_,-, 20,O,>, 20,O,>)/**/
/**/DELTA( 33, 34,_,<,  _,_,<,  _,_,<,  _,0,<,  _,1,<)/**/
/**/DELTA( 34, 35,_,<,  _,_,<,  _,_,<,  _,0,<,  _,1,<)/**/
/**/DELTA( 35, 39,_,>, 38,_,<, 38,_,<,  _,_,-,  _,_,-)/**/
/**/DELTA( 36, 37,_,>,  _,_,>,  _,_,>,  _,_,-,  _,_,-)/**/
/**/DELTA( 37, OK,_,-,  5,_,-,  5,_,-,  _,_,-,  _,_,-)/**/
/**/DELTA( 38,  3,_,>,  _,_,<,  _,_,<,  _,0,<,  _,1,<)/**/
/**/DELTA( 39,  3,_,>,  3,_,-,  3,_,-,  _,_,-,  _,_,-)/**/
/**/DELTA( 40, 41,_,<,  _,_,<,  _,_,<,  _,_,<,  _,_,<)/**/
/**/DELTA( 41, 13,_,>,  _,_,<,  _,_,<,  _,_,<,  _,_,<)
/********************************************************/


/***************************************************************************/
/**/END_STATES:                                                          /**/
/***************************************************************************/
/**/QOK: PRINT; printf("\n\n\t\t\t\033[1;32m Accept \033[0m") ; goto Q_ ;/**/
/**/QKO: PRINT; printf("\n\n\t\t\t\033[1;31m Reject \033[0m") ; goto Q_ ;/**/
/**/Q_: printf("done\n") ; return 0 ;                                    /**/
/***************************************************************************/
}
