/*
TLPI exercise 22-2
If both a realtime and a standard signal are pending for a process, SUSv3 leaves
it unspecified which is delivered first.  Write a program that shows what Linux
does in this case.  (Have the program set up a handler for all signals, block
signals for a period of time so that you can send various signals to it, and
then unblock all signals.)

compile:
    gcc sig_recv.c get_num.c -o sig_recv

*/

#define _GNU_SOURCE         /* needed for NSIG delcaration */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
//#include "get_num.h"

static volatile int sigCnt = 0;         /* Total number of signals received */
static volatile int allDone = 0;        /* Used to terminate program at end */

/******************************************************************************/
static void handler(int sig, siginfo_t *si, void *ucontext)
{
    /* UNSAFE: this handler uses non-async-signal-safe functions
       (printf())       */

    /* SIGINT or SIGTERM can be used to terminate program */
    if (sig == SIGINT || sig == SIGTERM) {
        allDone = 1;
        return;
    }

    sigCnt++;
    printf("Caught signal %d\n", sig);
    printf("    si_signo=%d, si_code=%d (%s), ", si->si_signo, si->si_code,
           (si->si_code == SI_USER)  ? "SI_USER"  :
           (si->si_code == SI_QUEUE) ? "SI_QUEUE" : "other");
    printf("si_value=%d\n", si->si_value.sival_int);
    printf("    si_pid=%ld, si_uid=%ld\n", (long) si->si_pid,
           (long) si->si_uid);

    return;
}

/******************************************************************************/
int main(int argc, char *argv[])
{
    struct sigaction sa;
    sigset_t emptymask;
    sigemptyset(&emptymask);        /* empty mask to return to later */

    sa.sa_sigaction = handler;
    sigfillset(&sa.sa_mask);        /* block all signals */
    sa.sa_flags = SA_SIGINFO;

    for (int n = 1; n < NSIG; n++) {
        if (n == SIGTSTP || n == SIGQUIT)
            continue;
        if (sigaction(n, &sa, NULL) == -1) {
            fprintf(stderr, "'sigaction' failed for n: %d!\n", n);
            //exit(EXIT_FAILURE);
        }
    }

    sigdelset(&sa.sa_mask, SIGINT);     /* unblock SIGINT   */
    sigdelset(&sa.sa_mask, SIGTERM);    /* unblock SIGTERM  */

    /*  set mask to block all but SIGINT and SIGTERM
        save old mask in prevmask   */
    if (sigprocmask(SIG_SETMASK, &sa.sa_mask, NULL) == -1) {
        fprintf(stderr, "'sigprocmask' failed!\n");
        exit(EXIT_FAILURE);
    }

    printf("Signals block, sleeping for 15 seconds to receive signals\n");
    sleep(15);

    printf("Sleep finished... unblocking signals!\n");
    if (sigprocmask(SIG_SETMASK, &emptymask, NULL) == -1) {
        fprintf(stderr, "'sigprocmask' failed!\n");
        exit(EXIT_FAILURE);
    }

    while (!allDone)        /* wait for incoming signals */
        pause();

    printf("Caught %d signals\n", sigCnt);
    exit(EXIT_SUCCESS);
}
