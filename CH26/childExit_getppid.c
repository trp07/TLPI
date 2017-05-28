/*
TLPI, exercise 26-1

Write a program to verify that when a child's parent terminates, a call to
getppid() returns 1 (the process ID of init).
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int
main(int argc, char *argv[])
{
    pid_t child, parent;

    setbuf(stdout, NULL);       /* set stdout to unbuffered */

    switch(fork()) {
        case -1:    /* fork() failed */
            fprintf(stderr, "fork failed!\n");
            exit(EXIT_FAILURE);

        case 0:     /* inside child */
            printf("    ... Inside Child ...\n");
            child = getpid();
            for(;;) {
                parent = getppid();
                printf("\tChild=%ld, parent=%ld\n",
                    (long) child, (long) parent);
                if (parent == 1) {
                    printf("\t[!] Parent died... 'init' is now parent!\n");
                    break;
                }
                sleep(1);   /* sleep to allow parent to exit */
            }
            printf("\tExiting Child\n");
            _exit(EXIT_SUCCESS);

        default:     /* inside parent */
            printf("*** Parent still alive (pid: %ld)\n", (long) getpid());
            sleep(3.5);
            printf("*** Parent exiting in 3 seconds\n");
            sleep(3);
            exit(EXIT_SUCCESS);

    }
}
