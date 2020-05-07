#include <stdio.h>

#define MAX_NB_SYM 62

enum err {
    OK,
    TOO_MANY_ARGS,
    TOO_FEW_ARGS,
    UNKNOWN_OPTION,
    WRONG_SYMBOL
};


int streq(char *, char *);
void err_too_many (int);
void err_too_few ();
void err_wrong_sym (int, char *);
void err_wrong_option (char *);
void print_help ();
int is_alpha (char);
int is_number (char);
void cpy (char *, char *, int *);
int is_valid_symlist (char *, int *);
int sym_identify (char);
void make_template (char *, int, int);
void repeat (char, int);


int main (int argc, char * argv []) {
    if (argc > 3) { err_too_many(argc); return TOO_MANY_ARGS; }
    if (argc == 1) { err_too_few(); return TOO_FEW_ARGS; }
    if (streq(argv[1], "-h")) {
        print_help();
        return OK;
    }
    char symbols [MAX_NB_SYM + 1];
    int cnt_sym = 0;
    int info;
    if (argc >= 2) {
        if (is_valid_symlist(argv[1], &info)) {
            cpy(symbols, argv[1], &cnt_sym);
        } else {
            err_wrong_sym(info, argv[1]);
            return WRONG_SYMBOL;
        }
    }
    int ternary = 1;
    if (argc == 3) {
        if (streq(argv[2], "-t")) {
            ternary = 0;
        } else {
            err_wrong_option(argv[1]); return UNKNOWN_OPTION;
        }
    }

    make_template(symbols, cnt_sym, ternary);
}

void err_too_many (int argc) {
    printf("You have given %d arguments, when the maximum is 2: a list of symbols and an option\n", argc);
}

void err_too_few () {
    printf("You have not provided any arguments.\nPlease provide a list of symbols).\n");
}

void err_wrong_sym (int info, char * arg) {
    if (info == 0) {
        printf("The `_` symbol is reserved for a blank tape cell.\nPlease use one of 0-9a-zA-Z instead.\n");
    } else if (info > 0) {
        printf("The symbol list you have provided is invalid:\n`%c` is not in 0-9a-zA-Z.\n", arg[info-1]);
    } else {
        printf("You have specified the symbol `%c` at least twice in your symbol list.\n", arg[-info-1]);
    }
}

void err_wrong_option (char * arg) {
    printf("`%s` is not a valid option. Use -h for more details.", arg);
}

void print_help () {
    printf("This is tmake, a command line tool for generating Turing machines.\n");
    printf("USAGE:\n");
    printf("    $ ./tmake (-h | SYMBOLS [-t])\n");
    printf("-h will display this message then exit\n");
    printf("SYMBOLS is a list of symbols in 0-9a-zA-Z (not separated by spaces)\n");
    printf("Optionally, -t will ensure that no GNU C extensions are used so that the resulting C program is compatible with all C compilers\n");
    printf("Example: `$ ./tmake 0abX -t` will output a machine with the five symbols `_` (blank), `0`, `a`, `b`, `X`, and no non-portable constructs.\n");
}

int is_alpha (char c) {
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

int is_number (char c) {
    return ('0' <= c && c <= '9');
}

void cpy (char * dest, char * source, int * len) {
    for (*len = 0; dest[*len] = source[*len]; (*len)++) ;
}

int streq(char * a, char * b) {
    while (*a && *b) {
        if (*a != *b) {
            return 0;
        }
        a++; b++;
    }
    if (*a == *b) {
        return 1;
    } else {
        return 0;
    }
}

int is_valid_symlist (char * arg, int * info) {
    int taken [MAX_NB_SYM] = {0};
    for (int i = 0; arg[i] != '\0'; i++) {
        if (arg[i] == '_') {
            *info = 0;
            return 0;
        } else if (!(is_alpha(arg[i]) || is_number(arg[i]))) {
            *info = i + 1;
            return 0;
        } else {
            int idx = sym_identify(arg[i]);
            if (taken[idx] == 0) {
                taken[idx] = 1;
            } else {
                *info = -i-1;
                return 0;
            }
        }
    }
    return 1;
}

int sym_identify (char s) {
    if ('0' <= s && s <= '9') {
        return s - '0';
    } else  if ('A' <= s && s <= 'Z') {
        return s - 'A' + 10;
    } else {
        return s - 'a' + 36;
    }
}

void make_template (char * symbols, int cnt, int ter) {
    printf("#include <stdio.h>\n\n");
    int idx [cnt+1];
    char sym [cnt+1];
    for (int i = 0; symbols[i] != '\0'; i++) {
        int id = sym_identify(symbols[i]);
        if (id <= 9) {
            idx[i+1] = id;
            sym[i+1] = symbols[i];
        } else {
            idx[i+1] = 8 - id;
            sym[i+1] = symbols[i];
        }
    }
    idx[0] = -1;
    sym[0] = '_';
    printf("enum SYMBOLS {");
    for (int i = 0; i <= cnt; i++) {
        if (idx[i] < 0) {
            printf("%c=%d, ", sym[i], idx[i]);
        }
    }
    printf("};\n");
    printf("\n#define x4 _,_,_,_\n");
    printf("#define x16 x4,x4,x4,x4\n");
    printf("#define x64 x16,x16,x16,x16\n");
    printf("#define x256 x64,x64,x64,x64\n");
    printf("#define OK 1000\n");
    printf("#define KO 1001\n\n");
    printf("int tape [] = { x256 } ;\n");
    printf("int * head = tape + 128 ;\n");
    printf("char tsl [] = \"");
    for (int i = '9'; i >= '0'; i--) { putchar(i); }
    putchar('_');
    for (int i = 'A'; i <= 'Z'; i++) { putchar(i); }
    for (int i = 'a'; i <= 'z'; i++) { putchar(i); }
    printf("\" ;\n\n");
    printf("#define PRINT printf(\"\\n\\n\\t\\t\");for(int idx=100;idx<140;idx++) printf(\"%%s%%c\", (head==tape+idx)?\"\\033[31m\":\"\\033[0m\",tsl[9-tape[idx]]);printf(\"\\033[0m\\n\")\n\n");
    printf("#define DELTA(q");
    for (int i = 0; i <= cnt; i++) {
        printf(", n%c,c%c,m%c", sym[i], sym[i], sym[i]);
    }
    printf(") \\\n\tQ##q: \\\n");
    printf("\tprintf(\"\\t{State %%d}\\t{read %%c}\\t\",q,tsl[9-*head]);for(int idx=100;idx<140;idx++) printf(\"%%s%%c\",(head==tape+idx)?\"\\033[31m\":\"\\033[0m\" ,tsl[9-tape[idx]]);printf(\"\\033[0m\\n\"); \\\n\tswitch(*head){ \\\n");
    for (int i = 0; i <= cnt; i++) {
        if (ter) {
            printf("\t\tcase %c:*head=(c%c==_?%c:c%c);head+=\"\\0\\2\\1\"[2 m%c 0]-1;(n%c==_)?({(c%c==_&&2 m%c 0==2)?({goto Q_;}):({goto Q##q;});}):({goto Q##n%c;}); \\\n", sym[i], sym[i], sym[i], sym[i], sym[i], sym[i], sym[i], sym[i], sym[i]);
        } else {
            printf("\t\tcase %c:*head=(c%c==_?%c:c%c);head+=\"\\0\\2\\1\"[2 m%c 0]-1;if(n%c==_){if(c%c==_&&2 m%c 0==2){goto Q_;}else{goto Q##q;}}else{goto Q##n%c;} \\\n", sym[i], sym[i], sym[i], sym[i], sym[i], sym[i], sym[i], sym[i], sym[i]);
        }
    }
    printf("\t}\n\nint main () {\n");
    printf("printf(\"  input > \");\n");
    printf("/* Enter here the initialisation logic */\n\n\n");
    printf("/******************/\n");
    printf("/**/START_STATE:/**/\n");
    printf("/******************/\n");
    printf("/**/  goto Q0;  /**/\n");
    printf("/******************/\n\n\n");
    int wth = 24 + 8*cnt;
    putchar('/'); repeat('*', wth); printf("/\n");
    printf("/**/TRANSITION_TABLE:"); repeat(' ', wth-23); printf("/**/\n");
    putchar('/'); repeat('*', wth); printf("/\n");
    printf("/*        q:  ");
    for (int i = 0; i <= cnt; i++) {
        printf(" %c:     ", sym[i]);
    }
    printf("  */\n");
    for (int i = 0; i < 10; i++) {
        printf("/**/DELTA(  ?");
        for (int i = 0; i <= cnt; i++) {
            printf(",  _,_,-");
        }
        printf(")/**/\n");
    }
    putchar('/'); repeat('*', wth); printf("/\n\n\n");
    putchar('/'); repeat('*', 75); printf("/\n");
    printf("/**/END_STATES:"); repeat(' ', 58); printf("/**/\n");
    putchar('/'); repeat('*', 75); printf("/\n");
    printf("/**/QOK: PRINT; printf(\"\\n\\n\\t\\t\\t\\033[1;32m Accept \\033[0m\") ; goto Q_ ;/**/\n");
    printf("/**/QKO: PRINT; printf(\"\\n\\n\\t\\t\\t\\033[1;31m Reject \\033[0m\") ; goto Q_ ;/**/\n");
    printf("/**/Q_: printf(\"done\\n\") ; return 0 ;                                    /**/\n");
    putchar('/'); repeat('*', 75); printf("/\n}\n");
}

void repeat(char c, int nb) {
    for (int i = 0; i < nb; i++) putchar(c);
}
