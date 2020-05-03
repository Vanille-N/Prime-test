#include <stdio.h>


#define MAX_LEN_NAME 10
#define MAX_NB_SYM 62


enum err {
    OK,
    TOO_FEW_ARGS,
    TOO_MANY_ARGS,
    WRONG_NAME,
    WRONG_SYMBOL
};


int streq(char *, char *);
void err_too_few ();
void err_too_many (int);
void err_wrong_name (int, char *);
void err_wrong_sym (int, char *);
void print_help ();
int is_valid_name (char *, int *);
int is_alpha (char);
int is_number (char);
void cpy (char *, char *, int *);
int is_valid_symlist (char *, int *);
int sym_identify (char);


int main (int argc, char * argv []) {
    printf("%d\n", argc);
    if (argc == 1) { err_too_few(); return TOO_FEW_ARGS; }
    if (argc > 3) { err_too_many(argc); return TOO_MANY_ARGS; }
    if (streq(argv[1], "-h")) {
        print_help();
        return OK;
    }
    char name [MAX_LEN_NAME + 1];
    int len_name = 0;
    char symbols [MAX_NB_SYM + 1];
    int cnt_sym = 0;
    int info;
    if (is_valid_name(argv[1], &info)) {
        cpy(name, argv[1], &len_name);
    } else {
        err_wrong_name(info, argv[1]);
        return WRONG_NAME;
    }
    if (argc == 3) {
        if (is_valid_symlist(argv[2], &info)) {
            cpy(symbols, argv[2], &cnt_sym);
        } else {
            err_wrong_sym(info, argv[2]);
            return WRONG_SYMBOL;
        }
    }


    // PLACEHOLDER
    printf("%s\n%s\n", name, symbols);
    return OK;

}


void err_too_few () {
    printf("You have not provided any arguments.\nPlease provide at least a name for your machine (and optionally a list of symbols).\n");
}

void err_too_many (int argc) {
    printf("You have given %d arguments, when the maximum is 2: a name and a list of symbols.", argc);
}

void err_wrong_name (int info, char * arg) {
    if (info == -1) {
        printf("`%s` is too long: the limit is %d characters.\n", arg, MAX_LEN_NAME);
    } else {
        printf("`%c` at position %d is invalid for a file identifier.\nPlease choose a name with format [a-zA-Z][a-zA-Z0-9_\\-]* instead.\n", arg[info], info);
    }
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

void print_help () {
    printf("/== PLACEHOLDER ==/\n");
}

int is_valid_name (char * arg, int * info) {
    if (!is_alpha(arg[0])) {
        *info = 0;
        return 0;
    }
    for (int i = 1; arg[i] != '\0'; i++) {
        if (i >= MAX_LEN_NAME) {
            *info = -1;
            return 0;
        }
        if (!(is_alpha(arg[i]) || is_number(arg[i]) || arg[i] == '_' || arg[i] == '-')) {
            *info = i;
            return 0;
        }
    }
    return 1;
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
