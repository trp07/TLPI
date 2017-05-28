/* TLPI, page 512

exercise 23-1:
Although alarm() is implemented as a system call within the Linux kernel,
this is redundant.  Implement alarm() using setitimer()

recall:

    unsigned int alarm(unsigned int seconds);
        Returns number of seconds remaining on any previous set timer, or
        0 if no timer previously set.

    int setitimer(int which, const struct itimerval *new_value,
                   struct itimerval *old_value);
        Returns 0 on success, -1 on error
        which = ITIMER_REAL, ITIMER_VIRTUAL, ITIMER_PROF

    struct itimerval {
        struct timeval it_interval;     // interval for periodic timer
        struct timeval it_value;        // Current value (time until next
                                            expiration)
    }

    struct timeval {
        time_t      tv_sec;     // seconds
        susecons_t  tv_usec;    // microseconds (long int)
    }

*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>

/*****************************************************************************/
static void sigAlrmHandler(int sig)
{
    printf("SIGALRM was sent!\t(hit Ctr-C to exit program)\n");
}
/*****************************************************************************/
int my_alarm(int seconds)
{
    struct itimerval itv;
    int curr_val;

    static int set = 0; /* alarm set sa_flags   */

    /* has alarm already been set?  if so, just return time remaining */
    if (set) {
        curr_val = itv.it_value.tv_sec + ((int)itv.it_value.tv_usec/1000000);
        return curr_val;
    }


    itv.it_interval.tv_sec = 0;
    itv.it_interval.tv_usec = 0;
    itv.it_value.tv_sec = seconds;
    itv.it_value.tv_usec = 0;

    if (setitimer(ITIMER_REAL, &itv, NULL) == -1) {
        fprintf(stderr, "setitimer failed!\n");
        exit(EXIT_FAILURE);
    }

    set = 1;
    curr_val = itv.it_value.tv_sec + ((int)itv.it_value.tv_usec/1000000);
    return curr_val;

}
/*****************************************************************************/
int main(int argc, char *argv[])
{
    struct sigaction sa;
    int seconds;
    int remaining;

    if (argc != 2) {
        fprintf(stderr, "Usage Error:\n\t%s  numSeconds\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    seconds = atoi(argv[1]);

    /* Change disposition of SIGALRM to go to a handler function since
       setitimer() will raise SIGALRM when set with argument which=ITIMER_REAL
    */
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigAlrmHandler;
    if (sigaction(SIGALRM, &sa, NULL) == -1) {
        fprintf(stderr, "sigaction failed!\n");
        exit(EXIT_FAILURE);
    }

    printf("Setting alarm with 'setitimer' for %d seconds\n", seconds);
    remaining = my_alarm(seconds);
    printf("Timer set... should go off in %d seconds\n", remaining);

    /* sleep a bit to see how much time remaining */
    sleep(seconds - 2);
    remaining = my_alarm(0);
    printf("Timer still set... should go off in %d seconds\n", remaining);

    /* wait for the signal to generate
       when SIGALRM is received, must Ctr-C (SIGINT) to end program
    */
    for(;;) {
        pause();
    }

    return 0;
}
