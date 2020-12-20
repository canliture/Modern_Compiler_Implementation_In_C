#include <stdio.h>
#include <stdlib.h>
#include "parse.h"

int main(int argc, char **argv) {
    if (argc!=2) {fprintf(stderr,"usage: a.out filename\n"); exit(1);}
    if (parse(argv[1])) {
        fprintf(stderr, "Parsing successful!\n");
    } else {
        fprintf(stderr,"Parsing failed\n");
    }
    return 0;
}