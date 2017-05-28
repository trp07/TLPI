/*
TLPI, exercise 20-2:
Write a program that shows that when the disposition of a pending signal is
changed to be SIG_IGN, the program never sees (catches) the signal.

Usage: ignore_pending_sig

   Type Control-C (^C) to generate a SIGINT signal after the program prints
   its "sleeping" message (see below).

*/

#define _GNU_SOURCE     /* needed for NSIG declaration - number that is one */
#include <signal.h>     /* greater than the highest signal number           */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/******************************************************************************/
static void handler(int sig)
{
    /* UNSAFE: This handler uses non-async-signal-safe functions
       (printf(), strsignal(), fflush(); see Section 21.1.2) */

    printf("Caught signal %d (%s)\n", sig, strsignal(sig));
    fflush(NULL);
}


/******************************************************************************/
void printSigset(FILE *of, const char *prefix, const sigset_t *sigset)
{
    int sig, cnt;
    cnt = 0;

    for (sig = 1; sig < NSIG; sig++) {
        if (sigismember(sigset, sig)) {
            cnt++;
            fprintf(of, "%s%d (%s)\n", prefix, sig, strsignal(sig));
        }
    }
    if (cnt == 0)
        fprintf(of, "%s<empty signal set>\n", prefix);
}

/******************************************************************************/
int main(int argc, char **argv)
{
    sigset_t pending, blocked;
    const int numSecs = 5;
    struct sigaction sa;

    /* Set up a handler for SIGINT */

    printf("Setting up handler for SIGINT\n");
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigaction(SIGINT, &sa, NULL) == -1)
        fprintf(stderr, "sigaction failed!\n");

    /* Block SIGINT for a while */

    sigemptyset(&blocked);
    sigaddset(&blocked, SIGINT);
    if (sigprocmask(SIG_SETMASK, &blocked, NULL) == -1)
        fprintf(stderr, "sigprocmask failed!\n");

    printf("BLOCKING SIGINT for %d seconds\n", numSecs);
    sleep(numSecs);

    /* Display mask of pending signals */

    if (sigpending(&pending) == -1)
        fprintf(stderr, "sigpending failed!\n");
    printf("PENDING signals are: \n");
    printSigset(stdout, "\t\t", &pending);

    /* Now ignore SIGINT */

    sleep(2);
    printf("Ignoring SIGINT\n");
    if (signal(SIGINT, SIG_IGN) == SIG_ERR)
        fprintf(stderr, "signal failed!\n");

    /* Redisplay mask of pending signals */

    if (sigpending(&pending) == -1)
        fprintf(stderr, "sigpending failed!\n");
    if (sigismember(&pending, SIGINT)) {
        printf("SIGINT is now pending\n");
    } else {
        printf("PENDING signals are: \n");
        printSigset(stdout, "\t\t", &pending);
    }
    sleep(2);

    /* Reestablish SIGINT handler */

    printf("Reestablishing handler for SIGINT\n");
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigaction(SIGINT, &sa, NULL) == -1)
        fprintf(stderr, "sigaction failed!\n");

    sleep(2);

    /* And unblock SIGINT */

    printf("UNBLOCKING SIGINT\n");
    sigemptyset(&blocked);
    if (sigprocmask(SIG_SETMASK, &blocked, NULL) == -1)
        fprintf(stderr, "sigprocmask failed!\n");

    exit(EXIT_SUCCESS);
}
