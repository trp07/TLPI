/*
TLPI, exercise 25-1

If a child process makes the call exit(-1), what exit status will be seen
by the parent?
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int
main(int argc, char *argv[])
{
    pid_t child;
    int status;

    printf("About to call fork()...\n");
    sleep(1);

    child = fork();
    switch (child) {
        case -1:    /* fork failed */
            fprintf(stderr, "fork failed!\n");
            exit(EXIT_FAILURE);

        case 0:     /* inside child */
            printf("    ...Inside child... about to call exit(-1)\n");
            sleep(1);
            _exit(-1);
            break;

        default:    /* inside parent */
            if (wait(&status) == -1) {
                fprintf(stderr, "wait failed!\n");
                exit(EXIT_SUCCESS);
            }

            /* use macros defined in <sys/wait.h> to get exit status */
            if (WIFEXITED(status))
                printf("Parent received exit status: %d\n", WEXITSTATUS(status));
            else
                printf("Parent did not receive exit status :(\n");
    }

    exit(EXIT_SUCCESS);
}
