/*
TLPI, sig_sender, page 412

sends any PID a sig-num n-times

compile:
    gcc sig_send.c get_num.c -o sig_send

usage:
    ./sig_sender PID num-sigs sig-num [sig-num-2]
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include "get_num.h"

int main(int argc, char *argv[])
{
    int numSigs, sig, j;
    pid_t pid;

    if (argc < 4 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s pid num-sigs sig-num [sig-num-2]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pid = getLong(argv[1], GN_GT_0, "PID");
    sig = getInt(argv[3], GN_GT_0, "Sig1");
    numSigs = getInt(argv[2], GN_GT_0, "NumSigs");


    /* send signals to receiver */
    printf("%s: sending signal %d to process %ld %d times\n",
            argv[0], sig, (long) pid, numSigs);

    for (j = 0; j < numSigs; j++) {
        if ( kill(pid, sig) == -1) {
            fprintf(stderr, "'kill' failed!\n");
            exit(EXIT_FAILURE);
        }
    }

    /* if a fourth command-line arg was specified, send that sig */

    if (argc > 4) {
        if ( kill(pid, getInt(argv[4], GN_GT_0, "Sig2")) == -1) {
            fprintf(stderr, "'kill' failed!\n");
            exit(EXIT_FAILURE);
        }
    }

    printf("%s: exiting\n", argv[0]);
    exit(EXIT_SUCCESS);
}
