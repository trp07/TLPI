/*
TLPI, Ch. 24, page 530

Modification of exercise 24-5.  Code that forks children, but each child
has to wait for the parent to "complete" code for the child to use after
forking.
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

/******************************************************************************/
static void
errExit(char *msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(EXIT_FAILURE);
}
/******************************************************************************/
static void
handler(int sig)
{
    /* sig handler does nothing but return */
    return;
}
/******************************************************************************/
int
getInt(char *arg)
{
    /* returns a command-line integer argument */
    char *endptr;
    long num = strtol(arg, &endptr, 10);
    return (int) num;
}
/******************************************************************************/
int
main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <numChildren>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int numChildren;
    pid_t childPid;

    /* Create signal masks */
    struct sigaction sa;
    sigset_t blockMask, origMask, emptyMask;
    sigemptyset(&blockMask);
    sigaddset(&blockMask, SIGUSR1);     /* block SIGUSR1  */
    if (sigprocmask(SIG_BLOCK, &blockMask, &origMask) == -1)
        errExit("sigprocmask failed!");

    /* set disposition of SIGUSR1 to hanlder function */
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = handler;
    if (sigaction(SIGUSR1, &sa, NULL) == -1)
        errExit("sigaction failed!");


    numChildren = getInt(argv[1]);
    printf("About to fork %d children\n", numChildren);
    setbuf(stdout, NULL);       /* disable buffering of stdout */

    for (int i = 0; i < numChildren; i++) {
        sleep(2);   /* wait a bit for each fork */
        switch (childPid = fork()) {
            case -1:
                errExit("fork failed!");

            case 0: /* child */
                printf("    ...Child %ld waiting for Parent = %ld\n",
                    (long) getpid(), (long) getppid());
                sigemptyset(&emptyMask);
                if (sigsuspend(&emptyMask) == -1 && errno != EINTR) {
                    fprintf(stderr, "sigsuspend failed in child: %ld\n",
                        (long) getpid());
                    _exit(EXIT_FAILURE);
                }
                printf("    ...Child %ld got SIGUSR1 signal!\n", (long) getpid());
                _exit(EXIT_SUCCESS);

            default: /* parent */
                printf("Parent (%ld) about to send signal to child %ld\n",
                    (long) getpid(), (long) childPid);
                sleep(1);
                printf("    +++Parent done with simulated work!\n");
                kill(childPid, SIGUSR1);
                wait(NULL);     /* wait for child to finish */
                break;
        }
    }


    exit(EXIT_SUCCESS);
}
