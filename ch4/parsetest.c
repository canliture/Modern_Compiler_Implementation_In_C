#include <stdio.h>
#include <stdlib.h>
#include "parse.h"
#include "prabsyn.h"

int main(int argc, char **argv) {
    if (argc!=2) {fprintf(stderr,"usage: a.out filename\n"); exit(1);}
    A_exp exp = parse(argv[1]);
    if (exp) {
        fprintf(stderr, "Parsing successful!\n");
        pr_exp(stdout, exp, 4);
        printf("\n");
    } else {
        fprintf(stderr,"Parsing failed\n");
    }
    return 0;
}