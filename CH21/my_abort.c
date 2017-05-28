/*
TLPI exercise 21-1:
implement abort()

** Not sure this was the intent of the problem since the my_abort() function
   raises a SIGABRT anyways.

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

/******************************************************************************/
void my_abort(void)
{
    /* call SIGABRT - should reroute to 'Handler'  */
    raise(SIGABRT);
    fprintf(stderr, "\tResetting SIGABRT to default disposition.\n");

    /* reset signal set to default */
    struct sigaction act;
    act.sa_handler = SIG_DFL;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGABRT, &act, NULL);

    /* call SIGABRT again   */
    raise(SIGABRT);
}
/******************************************************************************/
void Handler(int sig)
{
    fprintf(stderr, "SIGABRT received! - but handled!\n");
}
/******************************************************************************/
int main(int argc, char **argv)
{
    struct sigaction act;

    /* Create sigaction struct and reroute signal to Handler */
    act.sa_handler = Handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGABRT, &act, NULL);

    fprintf(stderr, "initiating abort in 3 seconds...\n");
    sleep(3);
    my_abort();

    fprintf(stderr, "Abort did not work!\n");
    exit(EXIT_FAILURE);

}
