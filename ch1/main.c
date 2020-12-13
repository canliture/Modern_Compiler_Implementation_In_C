/* This file is intentionally empty.  You should fill it in with your
   solution to the programming exercise. */

#include <stdio.h>
#include "prog1.h"
#include "interp.h"

int main(int argc, char** args) {
    A_stm p;

    p = prog();
    interp(p);
    printf("prog's maxArgs: %d\n", maxArgs(p));

    p = prog1();
    interp(p);
    printf("prog1's maxArgs: %d\n", maxArgs(p));

    return 0;
}