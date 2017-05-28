/*
TLPI, exercise 28-1

Write a program to see how fast fork() and vfork() system calls are on your
system.  Each child process should immediately exit, and the parent should
wait() on each child before creating the next.

Usage:
    ./time_forks -v|-f [numIters]

    -v denotes vfork()
    -f denotes  fork()
    numIters defaults to 1000 unless otherwise specified

Can also use in conjunction with the UNIX 'time' syscall...
    $ time ./time_forks -v|-f [numIters]

    example:  $ time ./time_forks -v 100000
              $ time ./time_forks -f 100000
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#include "get_num.h"

/* create function pointer */
pid_t (*func)(void);

/* function child process will execute */
int
childFunc(void)
{
    return 0;
}

/******************************************************************************/
int
main(int argc, char *argv[])
{
    pid_t child;
    int status, i, numIters;
    clock_t start;

    /* Usage information */
    if (argc < 2) {
        fprintf(stderr, "Usage error:\n\t%s -f|-v [numIters]\n"
                        "\t specify -v for vfork()\n"
                        "\t         -f for  fork()\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* determine if fork() or vfork() and point 'func' to it */
    if (strcmp(argv[1], "-v") == 0)
        func = vfork;
    else if (strcmp(argv[1], "-f") == 0)
        func = fork;
    else {
        fprintf(stderr, "Unrecognized function\n");
        exit(EXIT_FAILURE);
    }

    /* Number of iterations */
    numIters = (argc > 2) ? getInt(argv[2], GN_GT_0, "iterations") : 1000;

    /* create multiple iterations
       start timer first 
       (timer used if UNIX time syscall not available)  */
    start = clock();
    for (i = 0; i < numIters; i++) {
        child = func();
        switch(child) {
            case -1:    /* failed */
                     fprintf(stderr, "fork failed on iteration %d\n", i);
                     exit(EXIT_FAILURE);

            case 0:     /* child */
                     childFunc();
                     _exit(EXIT_SUCCESS);
                     break;

            default:    /* parent */
                     wait(&status);
        }
    }
    printf("Num children created: %d\n", i);
    printf("Total time: %f seconds\n",
           (float) (clock() - start) / CLOCKS_PER_SEC);
    exit(EXIT_SUCCESS);
}
