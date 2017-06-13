/*
TLPI, exercise 35-1

"implement the nice(1) command".

* Historically nice(1) was used to add a specified amount to the processes'
niceness value.

* Notice running the program as a normal user won't allow the program to change
nicess to a negative value; however, running as the superuser will succeed.

*/

#include <stdio.h>
#include <sys/resource.h>

/******************************************************************************/
int my_nice(int incr)
{
    /* adjusts the nicess value of a process by +incr.
       Returns -1 on failure.
       Returns new priority on success.
    */

    int prio = getpriority(PRIO_PROCESS, 0);
    if (setpriority(PRIO_PROCESS, 0, prio + incr) == -1)
        return -1;

    prio = getpriority(PRIO_PROCESS, 0);
    return prio;
}

/******************************************************************************/
int main(void)
{
    int prio = getpriority(PRIO_PROCESS, 0);
    printf("Current priority = %d\n", prio);

    printf("\nAdding +5 to the priority\n");
    my_nice(5);
    prio = getpriority(PRIO_PROCESS, 0);
    printf("Current priority = %d\n", prio);

    printf("\nAdding -7 to the priority\n");
    my_nice(-7);
    prio = getpriority(PRIO_PROCESS, 0);
    printf("Current priority = %d\n", prio);

    return 0;
}
